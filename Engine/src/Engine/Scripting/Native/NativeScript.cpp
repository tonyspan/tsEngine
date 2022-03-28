#include "pch.h"

#include "NativeScript.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/ScriptableEntity.h"

namespace tsEngine
{
	void NativeScript::Init(const Ref<EntityManager>& entityManager)
	{
		m_Context = entityManager;

		m_Context->GetAllEntitiesWith<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->EntityRef = entity;
				nsc.Instance->EntityManagerRef = m_Context;
				nsc.Instance->OnCreate();
			}
		});
	}

	void NativeScript::OnUpdate(float ts)
	{
		ASSERT(m_Context, "No context");

		m_Context->GetAllEntitiesWith<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->EntityRef = entity;
				nsc.Instance->EntityManagerRef = m_Context;
				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate(ts);
		});
	}

	void NativeScript::Shutdown()
	{
		ASSERT(m_Context, "No context");

		m_Context->GetAllEntitiesWith<NativeScriptComponent>().each([](auto entity, NativeScriptComponent& nsc)
		{
			if (nsc.Instance)
			{
				nsc.Instance->OnDestroy();
				nsc.Instance->EntityManagerRef = nullptr;
				nsc.DestroyScript(&nsc);
			}
		});
	}

	void NativeScript::UnloadScript(entt::entity entity)
	{
		ASSERT(m_Context, "No context");

		auto& nsc = m_Context->GetComponent<NativeScriptComponent>(entity);

		if (nsc.Instance)
		{
			nsc.Instance->OnDestroy();
			nsc.Instance->EntityManagerRef = nullptr;
			nsc.DestroyScript(&nsc);
		}

		m_Context->RemoveComponent<NativeScriptComponent>(entity);
	}
}