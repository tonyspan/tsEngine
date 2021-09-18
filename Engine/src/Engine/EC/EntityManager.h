#pragma once

#include "entt.hpp"

namespace tsEngine
{
	class EntityManager
	{
	public:
		entt::entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(entt::entity entity);

		template<typename T, typename... Args>
		T& AddComponent(entt::entity entity, Args&&... args)
		{
			assert(!HasComponent<T>(entity) && "Entity already has component");

			T& component = m_Registry.emplace<T>(entity, std::forward<Args>(args)...);

			return component;
		}

		template<typename T>
		T& GetComponent(entt::entity entity)
		{
			assert(HasComponent<T>(entity) && "Entity does not have component");

			return m_Registry.get<T>(entity);
		}

		template<typename T>
		bool HasComponent(entt::entity entity)
		{
			// No m_Registry.has<T> ?
			return m_Registry.any_of<T>(entity);
		}

		template<typename T>
		void RemoveComponent(entt::entity entity)
		{
			assert(HasComponent<T>() && "Entity does not have component");

			m_Registry.remove<T>(entity);
		}
	private:
		entt::registry m_Registry;
	};
}