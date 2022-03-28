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

	void EntityManager::DestroyEntityByTag(const std::string& tag)
	{
		auto e = FindEntityByTag(tag);

		if (e != entt::null)
			DestroyEntity(e);
	}

	entt::entity EntityManager::FindEntityByTag(const std::string& tag)
	{
		auto view = GetAllEntitiesWith<TagComponent>();

		for (const auto entity : view)
		{
			if (view.get<TagComponent>(entity).Tag == tag)
				return entity;
		}

		ASSERT(false, "Can not find entity by Tag: {}", tag);

		return entt::null;
	}

	void EntityManager::Clear()
	{
		m_Registry.clear();
	}

	void EntityManager::CopyRegistryAndComponents(Ref<EntityManager>& other)
	{
		m_Registry.clear();

		auto& src = other->m_Registry;
		m_Registry.assign(src.data(), src.data() + src.size(), src.destroyed());

		CopyComponent<TagComponent>(m_Registry, src);
		CopyComponent<TransformComponent>(m_Registry, src);
		CopyComponent<MoverComponent>(m_Registry, src);
		CopyComponent<SpriteComponent>(m_Registry, src);
		CopyComponent<CircleComponent>(m_Registry, src);
		CopyComponent<CameraComponent>(m_Registry, src);
		CopyComponent<CollisionComponent>(m_Registry, src);
		CopyComponent<TextComponent>(m_Registry, src);
	}
}