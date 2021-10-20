#include "pch.h"

#include "EntityManager.h"

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

	void EntityManager::ClearRegistry(entt::registry& registry)
	{
		registry.clear();
	}

	void EntityManager::CopyRegistryAndComponents(entt::registry& dst, entt::registry& src)
	{
		dst.assign(src.data(), src.data() + src.size(), src.destroyed());

		CopyComponent<TagComponent>(dst, src);
		CopyComponent<TransformComponent>(dst, src);
		CopyComponent<MoverComponent>(dst, src);
		CopyComponent<SpriteComponent>(dst, src);
		CopyComponent<CircleComponent>(dst, src);
		CopyComponent<CameraComponent>(dst, src);
		CopyComponent<CollisionComponent>(dst, src);
		CopyComponent<TextComponent>(dst, src);
	}
}