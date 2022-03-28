#include "Editor.h"

#include "imgui.h"

void Editor::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, const tsEngine::Ref<tsEngine::RenderManager>& renderManager)
{
	SetContext(entityManager, renderManager);
	m_HierarchyPanel.SetContext(m_EntityContext, m_RenderContext);

	m_EditorCamera = entityManager->CreateEntity("EditorCamera");
	entityManager->AddComponent<tsEngine::TransformComponent>(m_EditorCamera);
	entityManager->AddComponent<tsEngine::CameraComponent>(m_EditorCamera, 100.0f);

	m_EmptyEntityManager = tsEngine::CreateRef<tsEngine::EntityManager>();
	m_EmptyEntityManager->CopyRegistryAndComponents(m_EntityContext);
}

void Editor::SetContext(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, const tsEngine::Ref<tsEngine::RenderManager>& renderManager)
{
	m_EntityContext = entityManager;
	m_RenderContext = renderManager;
}

void Editor::OnUpdate()
{
	UpdateViewport();

	auto e = m_HierarchyPanel.GetSelectedEntity();

	// Position
	{
		if (tsEngine::Input::MouseButtonDown() && e != entt::null)
			m_EntityContext->GetComponent<tsEngine::TransformComponent>(e).Position = m_MousePos - m_ClickOffset;
	}

	// Size
	{
		bool control = tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::LeftCtrl) || tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::RightCtrl);

		auto wheelDir = tsEngine::Input::MouseWheelDirection();

		if (control && e != entt::null)
		{
			const auto offset = 10.0f;
			auto& size = m_EntityContext->GetComponent<tsEngine::TransformComponent>(e).Size;

			if (wheelDir == 1)
				size += offset;
			else if (wheelDir == -1)
				size -= offset;

			tsEngine::Input::ResetWheel();
		}
	}
}

void Editor::UpdateViewport()
{
	m_RenderContext->GetRenderer()->SetViewport(m_Viewport);

	auto view = m_EntityContext->GetAllEntitiesWith<tsEngine::TagComponent, tsEngine::CameraComponent, tsEngine::TransformComponent>();
	for (auto entity : view)
	{
		auto& tc = view.get<tsEngine::TransformComponent>(entity);
		
		//tc.Position = { m_Viewport.x, m_Viewport.y };
		tc.Size = { m_Viewport.z, m_Viewport.w };
	}
}

void Editor::New()
{
	m_EntityContext->Clear();
	m_EntityContext->CopyRegistryAndComponents(m_EmptyEntityManager);

	m_HierarchyPanel.SetContext(m_EntityContext, m_RenderContext);
	m_HoveredEntity = entt::null;
}

void Editor::Save()
{
	if (m_EditorLoadPath.empty())
	{
		m_EditorLoadPath = tsEngine::FileDialogs::SaveFileDialog("", tsEngine::AssetManager::s_AssetPath.string(), "*.tse\0");
	}

	if (!m_EditorLoadPath.empty())
	{
		tsEngine::Serializer serializer(m_EntityContext);
		serializer.Serialize(m_EditorLoadPath.string());
	}

}

void Editor::Load()
{
	if (m_EditorLoadPath.empty())
	{
		auto path = tsEngine::FileDialogs::OpenFileDialog("", tsEngine::AssetManager::s_AssetPath.string(), "*.tse\0");
		if (!path.empty())
			m_EditorLoadPath = path;
	}
	
	if(!m_EditorLoadPath.empty())
	{
		if (m_EditorLoadPath.extension().string() != ".tse")
		{
			LOG_WARN("Could not load {} - not a workspace file", m_EditorLoadPath.string());
			return;
		}

		m_EntityContext->Clear();
		tsEngine::Serializer serializer(m_EntityContext);

		if (serializer.Deserialize(m_EditorLoadPath.string()))
		{
			m_HierarchyPanel.SetContext(m_EntityContext, m_RenderContext);
			m_HoveredEntity = entt::null;
		}
	}

	m_EditorLoadPath.clear();
}

void Editor::OnImGuiRender()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_PassthruCentralNode);
	if (ImGui::BeginMainMenuBar())
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

		ImGui::EndMainMenuBar();

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar;
		bool open = true;
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", &open, windowFlags);

		auto m_ViewportFocused = ImGui::IsWindowFocused();
		auto m_ViewportHovered = ImGui::IsWindowHovered();
		tsEngine::Application::Get().GetImGui()->BypassImGuiHandleEvents(m_ViewportFocused && m_ViewportHovered);

		auto viewportSize = ImGui::GetContentRegionAvail();

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;

		const auto& imGuiTex = m_RenderContext->GetRenderer()->GetImGuiTexture();
		auto imGuiTexSize = imGuiTex->Size();
		ImGui::Image((ImTextureID)(intptr_t)imGuiTex->Raw(), { imGuiTexSize.x, imGuiTexSize.y });

		m_Viewport = { vMin.x, vMin.y, viewportSize.x, viewportSize.y };

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				m_EditorLoadPath = std::filesystem::path(tsEngine::AssetManager::s_BasePath) / path;

				Load();
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopStyleVar();
		ImGui::End();

		ImGui::Begin("Info");

		std::string name = "None";
		if (m_HoveredEntity != entt::null)
			name = m_EntityContext->GetComponent<tsEngine::TagComponent>(m_HoveredEntity).Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		const auto& appStats = tsEngine::Application::Get().GetStats();
		ImGui::TextWrapped("App Info:\nFPS: %.2f\nTimestep: %f", appStats.FPS, appStats.Timestep);
		const auto& renderStats = m_RenderContext->GetRenderer()->GetStats();
		ImGui::TextWrapped("Renderer Info:\nName: %s\nRendered Entities: %d", renderStats.RendererInfo->name, renderStats.RenderedEntities);
		ImGui::TextWrapped("How to use:\n- Load a file (*.tse) fast by dragging it from Content Browser to the Viewport.\n- Right click on Scene Hierarchy to create a new entity.\n- Click on Viewport to gain focus, select an entity and drag it around.");
		ImGui::TextWrapped("Shortcuts:\nCtrl+N: New workspace\nCtrl+O: Open file\nCtrl+S: Save current workspace\nCtrl+WheelUp/Down: Change the size of selected entity");

		ImGui::End();
	}

	m_HierarchyPanel.OnImGuiRender();
	m_ContentPanel.OnImGuiRender();
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
		if (event.ButtonId == tsEngine::MouseCode::LMB)
			m_HierarchyPanel.SetSelectedEntity(m_HoveredEntity);
	}
}

void Editor::OnMouseMoveEvent(tsEngine::MousePositionEvent& event)
{
	const auto& camera = m_RenderContext->GetCamera();

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
