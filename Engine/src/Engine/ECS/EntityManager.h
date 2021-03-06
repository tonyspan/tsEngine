#pragma once

#include "Engine/Log/Log.h"

#include "Components.h"

#include "entt.hpp"

class HierarchyPanel;

namespace tsEngine
{
	class EntityManager
	{
	public:
		EntityManager() = default;
		~EntityManager() = default;

		entt::entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(entt::entity entity);
		void DestroyEntityByTag(const std::string& tag);

		template<typename T, typename... Args>
		void AddComponent(entt::entity entity, Args&&... args)
		{
			ASSERT((!std::is_same<T, NativeScriptComponent>::value), "Add NativeScriptComponent with AddNativeScript()");
			ASSERT(!HasComponent<T>(entity), "Entity already has component");

			// Can't return component ?
			/*T& component =  */m_Registry.emplace<T>(entity, std::forward<Args>(args)...);

			//return component;
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

		template<typename T>
		auto& AddNativeScript(entt::entity entity)
		{
			ASSERT((std::is_same<T, NativeScriptComponent>::value), "Component is not a NativeScriptComponent");
			
			m_Registry.emplace<T>(entity);

			return m_Registry.get<NativeScriptComponent>(entity);
		}

		// NOTE: Leftover from a different approach
		auto& GetRegistry()
		{
			return m_Registry;
		}

		void Clear();

		void CopyRegistryAndComponents(Ref<EntityManager>& other);
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