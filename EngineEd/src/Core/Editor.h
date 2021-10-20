#pragma once

#include "Engine.h"

#include "HierarchyPanel.h"
#include "ContentPanel.h"

class Editor
{
public:
	void OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, const tsEngine::Ref<tsEngine::RenderManager>& renderManager);
	void OnUpdate();
	void OnImGuiRender();
	void OnEvent(tsEngine::Event& event);
private:
	void SetContext(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, const tsEngine::Ref<tsEngine::RenderManager>& renderManager);

	void OnKeyPressedEvent(tsEngine::KeyboardEvent& event);
	void OnMousePressedEvent(tsEngine::MouseButtonEvent& event);
	void OnMouseMoveEvent(tsEngine::MousePositionEvent& event);

	// Go through all entities with Sprite, Circle, Text component and see which is hovered
	template<typename T>
	bool CheckEntityIfHoveredWith(const tsEngine::CameraData& camera, const glm::vec2& mousePos)
	{
		auto view = m_Context->GetAllEntitiesWith<tsEngine::TransformComponent, T>();

		for (auto entity : view)
		{
			auto& transform = view.get<tsEngine::TransformComponent>(entity);

			if (IsEntityHovered(transform, camera, m_MousePos))
			{
				m_HoveredEntity = entity;
				return true;
			}
		}

		return false;
	}

	bool IsEntityHovered(const tsEngine::TransformComponent& transform, const tsEngine::CameraData& camera, const glm::vec2& mousePos);

	void CalcViewport();

	void New();
	void Save();
	void Load();
private:
	entt::entity m_EditorCamera;

	entt::entity m_HoveredEntity;

	tsEngine::Ref<tsEngine::EntityManager> m_Context;
	tsEngine::Ref<tsEngine::RenderManager> m_Context2;

	// For when creating blank workspace (i.e when loading save file)
	tsEngine::Ref<tsEngine::EntityManager> m_EmptyEntityManager;

	// For drag n drop (i.e move entities)
	glm::vec2 m_MousePos;
	glm::vec2 m_ClickOffset;

	SDL_Rect m_Viewport;

	// Panels
	HierarchyPanel m_HierarchyPanel;
	ContentPanel m_AssetPanel;

	std::string m_EditorLoadPath;
};