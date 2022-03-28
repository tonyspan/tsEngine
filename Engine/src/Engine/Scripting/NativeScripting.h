#pragma once

#include "Engine/Core/Base.h"
#include "entt.hpp"

namespace tsEngine
{
	class EntityManager;
	struct NativeScriptComponent;

	// Compile time scripting
	class NativeScripting
	{
	public:
		NativeScripting(const Ref<EntityManager>& entityManager);
		~NativeScripting() = default;

		void Init();
		void OnUpdate(float ts);
		void Shutdown();

		template<typename T>
		void LoadScript(entt::entity entity)
		{
			auto& nsc = m_Context->AddNativeScript<NativeScriptComponent>(entity);

			nsc.Bind<T>();

			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->EntityRef = entity;
				nsc.Instance->EntityManagerRef = m_Context;
				nsc.Instance->OnCreate();
			}
		}

		void UnloadScript(entt::entity entity);
	private:
		Ref<EntityManager> m_Context;
	};
}