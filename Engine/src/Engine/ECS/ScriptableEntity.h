#pragma once

#include "entt.hpp"

namespace tsEngine
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}
		
		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			if(!RegistryRef->any_of<T>(EntityRef))
				RegistryRef->emplace<T>(EntityRef, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			if (RegistryRef->any_of<T>(EntityRef))
				return RegistryRef->get<T>(EntityRef);
		}

		template<typename T>
		T& GetComponent(entt::entity entity)
		{
			if (RegistryRef->any_of<T>(EntityRef))
				return RegistryRef->get<T>(entity);
		}

		template<typename T>
		bool HasComponent()
		{
			return RegistryRef->any_of<T>(EntityRef);
		}
		template<typename T>
		void RemoveComponent()
		{
			if (RegistryRef->any_of<T>(EntityRef))
				RegistryRef->remove<T>(EntityRef);
		}

		// NOTE: None of the following should be public
		
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(float ts) {}
		entt::entity EntityRef = entt::null;
		entt::registry* RegistryRef = nullptr;
	};
}