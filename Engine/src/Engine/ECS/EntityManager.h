#pragma once

#include "Engine/Log/Log.h"

#include "entt.hpp"

class HierarchyPanel;

namespace tsEngine
{
	struct NativeScriptComponent;

	class EntityManager
	{
	public:
		EntityManager() = default;
		~EntityManager() = default;

		entt::entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(entt::entity entity);
		void DestroyEntityByTag(const std::string& tag);

		template<typename T, typename... Args>
		decltype(auto) AddComponent(entt::entity entity, Args&&... args)
		{
			ASSERT(!HasComponent<T>(entity), "Entity already has component");

			return m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent(entt::entity entity)
		{
			ASSERT(HasComponent<T>(entity), "Entity does not have component");

			return m_Registry.get<T>(entity);
		}

		template<typename T>
		bool HasComponent(entt::entity entity)
		{
			return m_Registry.any_of<T>(entity);
		}

		template<typename T>
		void RemoveComponent(entt::entity entity)
		{
			ASSERT(HasComponent<T>(entity), "Entity does not have component");

			m_Registry.remove<T>(entity);
		}

		template<typename... Args>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Args...>();
		}

		entt::entity FindEntityByTag(const std::string& tag);

		// NOTE: Leftover from a different approach
		auto& GetRegistry() { return m_Registry; }

		void Clear();

		void CopyFrom(const Ref<EntityManager>& from);

		// Set an entityManager as context to other subsystems
		static void SetActive(const Ref<EntityManager>& entityManager);
		static const Ref<EntityManager> GetActive();
	private:
		template<typename T>
		void CopyComponent(entt::registry& dst, entt::registry& src)
		{
			auto view = src.view<T>();

			for (auto e : view)
			{
				auto& component = view.get<T>(e);
				dst.emplace_or_replace<T>(e, component);
			}
		}
	private:
		entt::registry m_Registry;

		friend class HierarchyPanel;
		friend class Serializer;
	};
}