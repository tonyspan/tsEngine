#include "pch.h"

#include "EntityManager.h"

#include "Components.h"

namespace tsEngine
{
	static Ref<EntityManager> s_ActiveEntityManager;

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

	void EntityManager::CopyFrom(const Ref<EntityManager>& from)
	{
		auto& dst = m_Registry;
		dst.clear();

		auto& src = from->m_Registry;
		
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
	void EntityManager::SetActive(const Ref<EntityManager>& entityManager)
	{
		s_ActiveEntityManager = entityManager;
	}
	
	const Ref<EntityManager> EntityManager::GetActive()
	{
		ASSERT(s_ActiveEntityManager, "Use EntityManager::SetActive first");

		return s_ActiveEntityManager;
	}
}