#include "Editor.h"

void Editor::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, const tsEngine::Ref<tsEngine::RenderManager>& renderManager)
{
    ASSERT(entityManager != nullptr, "Must pass valid pointer to entityManager to Editor::OnCreate");
    ASSERT(renderManager != nullptr, "Must pass valid pointer to entityManager to Editor::OnCreate");

    m_EditorCamera = entityManager->CreateEntity("MainCamera");
    entityManager->AddComponent<tsEngine::TransformComponent>(m_EditorCamera);
    entityManager->AddComponent<tsEngine::CameraComponent>(m_EditorCamera, 100.0f);

    SetContext(entityManager, renderManager);
    m_HierarchyPanel.SetContext(entityManager, renderManager);

    // Save the default/blank workspace to be loaded when a new one is created
    m_EmptyEntityManager = tsEngine::CreateRef<tsEngine::EntityManager>();
    tsEngine::EntityManager::CopyRegistryAndComponents(m_EmptyEntityManager->GetRegistry(), m_Context->GetRegistry());
}

void Editor::SetContext(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, const tsEngine::Ref<tsEngine::RenderManager>& renderManager)
{
    m_Context = entityManager;
    m_Context2 = renderManager;
}

void Editor::OnUpdate()
{
    CalcViewport();
}

// NOTE: Hardcoded viewport
void Editor::CalcViewport()
{
    m_Viewport = { 420, 15, 1170, 580 };
    m_Context2->GetRenderer()->SetViewport(m_Viewport);

    auto view = m_Context->GetAllEntitiesWith<tsEngine::TagComponent, tsEngine::CameraComponent, tsEngine::TransformComponent>();

    for (auto entity : view)
    {
        view.get<tsEngine::TransformComponent>(entity).Size = { m_Viewport.w, m_Viewport.h };
    }
}

void Editor::New()
{
    tsEngine::EntityManager::ClearRegistry(m_Context->GetRegistry());
    tsEngine::EntityManager::CopyRegistryAndComponents(m_Context->GetRegistry(), m_EmptyEntityManager->GetRegistry());

    m_HierarchyPanel.SetContext(m_Context, m_Context2);
    m_HoveredEntity = entt::null;
}

void Editor::Save()
{    
    if (!m_EditorLoadPath.empty())
    {
        tsEngine::Serializer serializer(m_Context);
        serializer.Serialize(m_EditorLoadPath);
        //m_EditorLoadPath = filepath;
    }
}

void Editor::Load()
{   
    tsEngine::EntityManager::ClearRegistry(m_Context->GetRegistry());
    
    tsEngine::Serializer serializer(m_Context);

    // NOTE: Temporary
    if(m_HierarchyPanel.GetFileToBeDeserialized().empty())
        m_EditorLoadPath = tsEngine::AssetManager::s_AssetPath.string() + "entities.tse";
    
    if (serializer.Deserialize(m_EditorLoadPath))
    {
        m_HierarchyPanel.SetContext(m_Context, m_Context2);
        m_HoveredEntity = entt::null;
    }
}

void Editor::OnImGuiRender()
{
    bool active = false;

    const ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;

    ImGui::Begin("Main", &active, winFlags);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New...", "Ctrl+N"))
                New();
            if (ImGui::MenuItem("Load...", "Ctrl+O"))
                Load();
            if (ImGui::MenuItem("Save...", "Ctrl+S"))
                Save();
            if (ImGui::MenuItem("Quit"))
                tsEngine::Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::Text("FPS: %.2f", tsEngine::Application::Get().GetStats().FPS);
        ImGui::Text("Hovered Entity: %s", m_HoveredEntity != entt::null ? m_Context->GetComponent<tsEngine::TagComponent>(m_HoveredEntity).Tag.c_str() : "-");

        ImGui::EndMenuBar();
    }

    // NOTE: Is it the right place for this?
    if (!m_HierarchyPanel.GetFileToBeDeserialized().empty())
    {
        m_EditorLoadPath = m_HierarchyPanel.GetFileToBeDeserialized().string();
        Load();
        m_HierarchyPanel.GetFileToBeDeserialized().clear();
    }

    m_HierarchyPanel.OnImGuiRender();
    m_AssetPanel.OnImGuiRender();

    ImGui::End();
}

void Editor::OnEvent(tsEngine::Event& event)
{
    tsEngine::EventDispatcher dispatcher(&event);

    dispatcher.Dispatch<tsEngine::KeyboardEvent>(ENGINE_BIND_FUNC(OnKeyPressedEvent));
    dispatcher.Dispatch<tsEngine::MouseButtonEvent>(ENGINE_BIND_FUNC(OnMousePressedEvent));
    dispatcher.Dispatch<tsEngine::MousePositionEvent>(ENGINE_BIND_FUNC(OnMouseMoveEvent));
}

void Editor::OnKeyPressedEvent(tsEngine::KeyboardEvent& event)
{
    // Shortcuts
    if (event.Repeat > 0)
        return;

    bool control = tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::LeftCtrl) || tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::RightCtrl);

    switch (event.Scancode)
    {
        case tsEngine::KeyCode::N:
        {
            if (control)
                New();
        
            break;
        }
        case tsEngine::KeyCode::O:
        {
            if (control)
                Load();

            break;
        }
        case tsEngine::KeyCode::S:
        {
            if (control)
                Save();

            break;
        }
    }
}

void Editor::OnMousePressedEvent(tsEngine::MouseButtonEvent& event)
{
    if (event.Down)
    {
        if (event.ButtonId == tsEngine::MouseButtonId::LMB)
            m_HierarchyPanel.SetSelectedEntity(m_HoveredEntity);
    }
}

void Editor::OnMouseMoveEvent(tsEngine::MousePositionEvent& event)
{
    const auto& camera = m_Context2->GetCamera();

    m_MousePos = tsEngine::Input::MousePosition();

    auto spriteEntitytHovered = CheckEntityIfHoveredWith<tsEngine::SpriteComponent>(camera, m_MousePos);

    if (spriteEntitytHovered)
        return;

    auto circleEntitytHovered = CheckEntityIfHoveredWith<tsEngine::CircleComponent>(camera, m_MousePos);

    if (circleEntitytHovered)
        return;

    auto textEntitytHovered = CheckEntityIfHoveredWith<tsEngine::TextComponent>(camera, m_MousePos);

    if (textEntitytHovered)
        return;

    m_HoveredEntity = entt::null;

    if (tsEngine::Input::MouseButtonDown())
        m_Context->GetComponent<tsEngine::TransformComponent>(m_HierarchyPanel.GetSelectedEntity()).Position = m_MousePos - m_ClickOffset;

}

bool Editor::IsEntityHovered(const tsEngine::TransformComponent& transform, const tsEngine::CameraData& camera, const glm::vec2& mousePos)
{
    auto screenPosition = tsEngine::Renderer::GetScreenPosition(transform.Position, camera);

    SDL_Rect dst = { (int)(screenPosition.x - transform.Size.x / 2), (int)(screenPosition.y - transform.Size.y / 2), (int)transform.Size.x, (int)transform.Size.y };

    SDL_Point point = { (int)mousePos.x, (int)mousePos.y };

    if (SDL_PointInRect(&point, &dst))
    {
        m_ClickOffset = mousePos - transform.Position;

        return true;
    }

    return false;
}
