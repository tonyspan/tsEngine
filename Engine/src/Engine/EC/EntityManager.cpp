#include "pch.h"

#include "EntityManager.h"
#include "Engine/EC/Components.h"

namespace tsEngine
{
	entt::entity EntityManager::CreateEntity(const std::string& name)
	{
		entt::entity entity = m_Registry.create();
		
		m_Registry.emplace<TagComponent>(entity);
		
		auto& tag = m_Registry.get<TagComponent>(entity);
		
		tag.Tag = name.empty() ? "Entity" : name;
		
		return entity;
	}

	void EntityManager::DestroyEntity(entt::entity entity)
	{
		m_Registry.destroy(entity);
	}
}