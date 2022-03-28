#pragma once

#include "Engine.h"

#include "HierarchyPanel.h"
#include "ContentPanel.h"

class Editor
{
public:
	void OnCreate();
	void OnUpdate();
	void OnImGuiRender();
	void OnEvent(tsEngine::Event& event);
private:
	void OnKeyPressedEvent(tsEngine::KeyboardEvent& event);
	void OnMousePressedEvent(tsEngine::MouseButtonEvent& event);
	void OnMouseMoveEvent(tsEngine::MousePositionEvent& event);

	// Go through all entities with Sprite, Circle, Text component and see which is hovered
	template<typename T>
	bool CheckEntityIfHoveredWith(const tsEngine::CameraData& camera, const glm::vec2& mousePos)
	{
		auto entityManager = tsEngine::EntityManager::GetActive();
		auto view = entityManager->GetAllEntitiesWith<tsEngine::TransformComponent, T>();

		for (auto entity : view)
		{
			auto& transform = entityManager->GetComponent<tsEngine::TransformComponent>(entity);

			if (IsEntityHovered(transform.Position, transform.Size, camera, mousePos))
			{
				m_HoveredEntity = entity;
				return true;
			}
		}

		return false;
	}

	bool IsEntityHovered(const glm::vec2& position, const glm::vec2& size, const tsEngine::CameraData& camera, const glm::vec2& mousePos);

	void UpdateViewport();

	void New();
	void Save();
	void Load();

	void OnEditorPlay();
	void OnEditorStop();
private:
	entt::entity m_EditorCamera;

	entt::entity m_HoveredEntity;

	tsEngine::Ref<tsEngine::EntityManager> m_EntityManager;
	tsEngine::Ref<tsEngine::EntityManager> m_EmptyEntityManager;
	tsEngine::Ref<tsEngine::EntityManager> m_HelperEntityManager;

	// For drag n drop (i.e move entities)
	glm::vec2 m_MousePos;
	glm::vec2 m_ClickOffset;

	glm::vec4 m_Viewport;

	// Panels
	HierarchyPanel m_HierarchyPanel;
	ContentPanel m_ContentPanel;

	std::filesystem::path m_EditorLoadPath;

	enum class EditorState
	{
		Edit = 0,
		Play
	};

	EditorState m_State = EditorState::Edit;

	tsEngine::Ref<tsEngine::Texture> m_Play;
	tsEngine::Ref<tsEngine::Texture> m_Stop;
};