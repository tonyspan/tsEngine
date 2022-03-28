#include "pch.h"

#include "NativeScripting.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/ScriptableEntity.h"

namespace tsEngine
{
	NativeScripting::NativeScripting(const Ref<EntityManager>& entityManager)
		: m_Context(entityManager)
	{
	}

	void NativeScripting::Init()
	{
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

	void NativeScripting::OnUpdate(float ts)
	{
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

	void NativeScripting::Shutdown()
	{
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

	void NativeScripting::UnloadScript(entt::entity entity)
	{
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