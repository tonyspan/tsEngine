#include "Editor.h"

#include "imgui.h"

void Editor::OnCreate()
{
	m_EntityManager = tsEngine::CreateRef<tsEngine::EntityManager>();
	tsEngine::EntityManager::SetActive(m_EntityManager);
	m_HierarchyPanel.SetContext(m_EntityManager);

	m_EditorCamera = m_EntityManager->CreateEntity("EditorCamera");
	m_EntityManager->AddComponent<tsEngine::TransformComponent>(m_EditorCamera);
	m_EntityManager->AddComponent<tsEngine::CameraComponent>(m_EditorCamera, 100.0f);

	m_EmptyEntityManager = tsEngine::CreateRef<tsEngine::EntityManager>();
	m_EmptyEntityManager->CopyFrom(m_EntityManager);

	m_HelperEntityManager = tsEngine::CreateRef<tsEngine::EntityManager>();
	m_HelperEntityManager->CopyFrom(m_EntityManager);

	tsEngine::AssetManager::AddAsset<tsEngine::Texture>("play", tsEngine::Asset::Create<tsEngine::Texture>((tsEngine::AssetManager::s_AssetPath / "play.png").string()));
	tsEngine::AssetManager::AddAsset<tsEngine::Texture>("stop", tsEngine::Asset::Create<tsEngine::Texture>((tsEngine::AssetManager::s_AssetPath / "stop.png").string()));

	m_Play = tsEngine::AssetManager::GetAsset<tsEngine::Texture>("play");
	m_Stop = tsEngine::AssetManager::GetAsset<tsEngine::Texture>("stop");
}

void Editor::OnUpdate()
{
	UpdateViewport();

	// A way to simulate Play/Edit states
	switch (m_State)
	{
		case EditorState::Edit:
		{
			m_HelperEntityManager->CopyFrom(m_EntityManager);

			// These should be visible only on runtime
			{
				m_HelperEntityManager->GetAllEntitiesWith<tsEngine::MoverComponent>().each([&](auto entity, tsEngine::MoverComponent& mc)
				{
					m_HelperEntityManager->RemoveComponent<tsEngine::MoverComponent>(entity);
				});

				m_HelperEntityManager->GetAllEntitiesWith<tsEngine::SpriteComponent>().each([](auto entity, tsEngine::SpriteComponent& src)
				{
					src.HasAnimation = (src.HasAnimation ? !src.HasAnimation : src.HasAnimation);
				});
			}

			tsEngine::EntityManager::SetActive(m_HelperEntityManager);

			break;
		}
		
		case EditorState::Play:
		{
			break;
		}
	}

	auto e = m_HierarchyPanel.GetSelectedEntity();

	// Position
	{
		if (tsEngine::Input::MouseButtonDown() && e != entt::null)
			m_EntityManager->GetComponent<tsEngine::TransformComponent>(e).Position = m_MousePos - m_ClickOffset;
	}

	// Size
	{
		bool control = tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::LeftCtrl) || tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::RightCtrl);

		auto wheelDir = tsEngine::Input::MouseWheelDirection();

		if (control && e != entt::null)
		{
			const auto offset = 10.0f;
			auto& size = m_EntityManager->GetComponent<tsEngine::TransformComponent>(e).Size;

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
	tsEngine::RenderSystem::Get().GetRenderer()->SetViewport(m_Viewport);

	auto view = m_EntityManager->GetAllEntitiesWith<tsEngine::TagComponent, tsEngine::CameraComponent, tsEngine::TransformComponent>();
	for (auto entity : view)
	{
		auto& tc = m_EntityManager->GetComponent<tsEngine::TransformComponent>(entity);
		
		//tc.Position = { m_Viewport.x, m_Viewport.y };
		tc.Size = { m_Viewport.z, m_Viewport.w };
	}
}

void Editor::New()
{
	m_EntityManager->Clear();
	m_EntityManager->CopyFrom(m_EmptyEntityManager);

	m_HierarchyPanel.SetContext(m_EntityManager);
	m_HoveredEntity = entt::null;
}

// NOTE: Refactor
void Editor::Save()
{
	if (m_EditorLoadPath.empty())
	{
		m_EditorLoadPath = tsEngine::FileDialogs::SaveFileDialog("", tsEngine::AssetManager::s_AssetPath.string(), "*.tse");
	}

	if (!m_EditorLoadPath.empty())
	{
		tsEngine::Serializer serializer(m_EntityManager);
		serializer.Serialize(m_EditorLoadPath.string());
	}

}

void Editor::Load()
{
	if (m_EditorLoadPath.empty())
	{
		auto path = tsEngine::FileDialogs::OpenFileDialog("", tsEngine::AssetManager::s_AssetPath.string(), "*.tse");
		if (!path.empty())
			m_EditorLoadPath = path;
	}
	
	if(!m_EditorLoadPath.empty())
	{
		const auto asset = tsEngine::Asset::IsSupported(m_EditorLoadPath);
		if (asset.IsSupported && !(asset.Type == tsEngine::AssetType::Workspace))
		{
			LOG_WARN("Could not load {} - not a workspace file", m_EditorLoadPath.string());
			return;
		}

		m_EntityManager->Clear();
		tsEngine::Serializer serializer(m_EntityManager);

		if (serializer.Deserialize(m_EditorLoadPath.string()))
		{
			m_HierarchyPanel.SetContext(m_EntityManager);
			m_HoveredEntity = entt::null;
		}
	}

	m_EditorLoadPath.clear();
}

void Editor::OnEditorPlay()
{
	LOG_WARN("OnPlay");

	tsEngine::EntityManager::SetActive(m_EntityManager);
	m_HierarchyPanel.SetContext(m_EntityManager);

	m_State = EditorState::Play;
}

void Editor::OnEditorStop()
{
	LOG_WARN("OnEdit");

	m_HelperEntityManager->Clear();
	m_HelperEntityManager->CopyFrom(m_EntityManager);
	m_HierarchyPanel.SetContext(m_EntityManager);

	m_State = EditorState::Edit;
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
		tsEngine::Application::Get().GetImGui()->HandleEvents(m_ViewportFocused && m_ViewportHovered);

		auto viewportSize = ImGui::GetContentRegionAvail();

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;

		const auto& imGuiTex = tsEngine::RenderSystem::Get().GetRenderer()->GetRenderTargetTexture();
		auto imGuiTexSize = imGuiTex->Size();
		ImGui::Image((ImTextureID)(intptr_t)imGuiTex->As<SDL_Texture>(), { imGuiTexSize.x, imGuiTexSize.y });

		m_Viewport = { vMin.x, vMin.y, viewportSize.x, viewportSize.y };

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				auto filePath = std::filesystem::path(tsEngine::AssetManager::s_BasePath) / path;

				auto asset = tsEngine::Asset::IsSupported(filePath);

				auto selectedEntity = m_HierarchyPanel.GetSelectedEntity();

				if (asset.Type == tsEngine::AssetType::Workspace)
				{
					m_EditorLoadPath = filePath;
					Load();
				}
				else if (asset.Type == tsEngine::AssetType::Texture && selectedEntity != entt::null)
				{
					auto texture = tsEngine::Asset::Create<tsEngine::Texture>(filePath.string());
					tsEngine::AssetManager::AddAsset<tsEngine::Texture>(filePath.string(), texture);
					auto& src = m_EntityManager->GetComponent<tsEngine::SpriteComponent>(selectedEntity);
					src.Image = tsEngine::AssetManager::GetAsset<tsEngine::Texture>(filePath.string());
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopStyleVar();
		ImGui::End();

		ImGui::Begin("Info");

		std::string name = "None";
		if (m_HoveredEntity != entt::null)
			name = m_EntityManager->GetComponent<tsEngine::TagComponent>(m_HoveredEntity).Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		const auto& appStats = tsEngine::Application::Get().GetStats();
		ImGui::TextWrapped("App Info:\nFPS: %.2f\nTimestep: %f", appStats.FPS, appStats.Timestep);
		const auto& renderStats = tsEngine::RenderSystem::Get().GetRenderer()->GetStats();
		ImGui::TextWrapped("Renderer Info:\nName: %s\nRendered Entities: %d", renderStats.RendererInfo->name, renderStats.RenderedEntities);
		ImGui::TextWrapped("How to use:\n- Load a file (*.tse) fast by dragging it from Content Browser to the Viewport.\n- Right click on Scene Hierarchy to create a new entity.\n- Click on Viewport to gain focus, select an entity and drag it around.");
		ImGui::TextWrapped("Shortcuts:\nCtrl+N: New workspace\nCtrl+O: Open file\nCtrl+S: Save current workspace\nCtrl+WheelUp/Down: Change the size of selected entity");

		ImGui::End();

		ImGui::Begin("##PlayStop", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		
		auto icon = (m_State == EditorState::Edit ? m_Play : m_Stop);
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - 0.5f);
		if (ImGui::ImageButton((ImTextureID)(intptr_t)icon->As<SDL_Texture>(), ImVec2(25, 25)))
		{
			m_State == EditorState::Edit ? OnEditorPlay() : OnEditorStop();
		}
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
	const auto& camera = tsEngine::RenderSystem::Get().GetCamera();

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

bool Editor::IsEntityHovered(const glm::vec2& position, const glm::vec2& size, const tsEngine::CameraData& camera, const glm::vec2& mousePos)
{
	auto screenPosition = tsEngine::Renderer::GetScreenPosition(position, camera);

	SDL_Rect dst = { (int)(screenPosition.x - size.x / 2), (int)(screenPosition.y - size.y / 2), (int)size.x, (int)size.y };

	SDL_Point point = { (int)mousePos.x, (int)mousePos.y };

	if (SDL_PointInRect(&point, &dst))
	{
		m_ClickOffset = mousePos - position;

		return true;
	}

	return false;
}
