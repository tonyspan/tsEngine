#pragma once

#include "Engine.h"

#include <glm/gtc/type_ptr.hpp>

class HierarchyPanel
{
public:
	HierarchyPanel() = default;
	HierarchyPanel(const tsEngine::Ref<tsEngine::EntityManager>& context, const tsEngine::Ref<tsEngine::RenderManager>& context2);

	void SetContext(const tsEngine::Ref<tsEngine::EntityManager>& context, const tsEngine::Ref<tsEngine::RenderManager>& context2);

	void OnImGuiRender();

	void SetSelectedEntity(entt::entity entity);
	entt::entity GetSelectedEntity() const { return m_SelectedEntity; }

	std::filesystem::path& GetFileToBeDeserialized() { return m_FileToBeDeserialized; }
private:
	void DrawEntityNode(entt::entity entity);
	void DrawComponents(entt::entity entity);

	void EntityHierarchy();
	void Properties();
private:
	tsEngine::Ref<tsEngine::EntityManager> m_Context;
	tsEngine::Ref<tsEngine::RenderManager> m_Context2;
	entt::entity m_SelectedEntity;

	std::filesystem::path m_FileToBeDeserialized;
};
