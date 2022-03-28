#pragma once

#include "Engine/Core/Base.h"

#include "entt.hpp"

namespace tsEngine
{
	class EntityManager;
	struct NativeScriptComponent;

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;
	protected:
		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			if (!EntityManagerRef->HasComponent<T>(EntityRef))
				EntityManagerRef->AddComponent<T>(EntityRef, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return EntityManagerRef->GetComponent<T>(EntityRef);
		}

		template<typename T>
		T& GetComponent(entt::entity entity)
		{
			return EntityManagerRef->GetComponent<T>(entity);
		}

		template<typename T>
		bool HasComponent()
		{
			return EntityManagerRef->HasComponent<T>(EntityRef);
		}

		template<typename T>
		void RemoveComponent()
		{
			EntityManagerRef->RemoveComponent<T>(EntityRef);
		}
	private:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(float ts) {}
		entt::entity EntityRef = entt::null;
		Ref<EntityManager> EntityManagerRef = nullptr;

		friend class NativeScript;
	};
}