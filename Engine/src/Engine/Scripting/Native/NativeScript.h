#pragma once

#include "Engine/Core/Base.h"
#include "entt.hpp"

namespace tsEngine
{
	class EntityManager;
	struct NativeScriptComponent;

	// Compile time scripting
	class NativeScript
	{
	public:
		static void Init(const Ref<EntityManager>& entityManager);
		static void OnUpdate(float ts);
		static void Shutdown();

		template<typename T>
		static void LoadScript(entt::entity entity)
		{
			auto& nsc = m_Context->AddComponent<NativeScriptComponent>(entity);

			nsc.Bind<T>();

			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->EntityRef = entity;
				nsc.Instance->EntityManagerRef = m_Context;
				nsc.Instance->OnCreate();
			}
		}

		static void UnloadScript(entt::entity entity);
	private:
		static inline Ref<EntityManager> m_Context;
	};
}