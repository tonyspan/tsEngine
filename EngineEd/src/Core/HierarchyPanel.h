#pragma once

#include "Engine.h"

#include <glm/gtc/type_ptr.hpp>

class HierarchyPanel
{
public:
	HierarchyPanel() = default;
	HierarchyPanel(const tsEngine::Ref<tsEngine::EntityManager>& entityManager);

	void SetContext(const tsEngine::Ref<tsEngine::EntityManager>& entityManager);

	void OnImGuiRender();

	void SetSelectedEntity(entt::entity entity);
	entt::entity GetSelectedEntity() const { return m_SelectedEntity; }
private:
	void DrawEntityNode(entt::entity entity);
	void DrawComponents(entt::entity entity);

	template<typename T>
	void DrawComponentItem(const std::string& componentName, entt::entity entity, std::function<void(tsEngine::Ref<tsEngine::EntityManager>&, entt::entity)> callback = {})
	{
		if (ImGui::MenuItem(componentName.c_str()))
		{
			if (!m_Context->HasComponent<T>(entity))
				m_Context->AddComponent<T>(entity);
			else
				LOG_WARN("This entity already has the {}", componentName);
			ImGui::CloseCurrentPopup();
		}

		if (callback && m_Context->HasComponent<T>(entity))
			callback(m_Context, entity);
	}

	void EntityHierarchy();
	void Properties();
private:
	tsEngine::Ref<tsEngine::EntityManager> m_Context;

	entt::entity m_SelectedEntity = entt::null;
};
