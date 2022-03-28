#include "pch.h"

#include "RenderSystem.h"

#include "Renderer.h"
#include "Camera.h"

#include "Engine/Log/Log.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"

#include "Engine/Window/Window.h"

namespace tsEngine
{
	RenderSystem::RenderSystem(Window* window)
	{
		LOG_INFO("Initializing RenderSystem");

		s_Instance = this;

		m_Renderer = CreateScope<Renderer>(window);
	}

	RenderSystem::~RenderSystem()
	{
		LOG_INFO("Shutting down RenderSystem");
	}

	void RenderSystem::OnUpdate(const Ref<EntityManager>& entityManager, const ImGuiCallback& callback)
	{
		m_Renderer->BeginScene();

		auto view = entityManager->GetAllEntitiesWith<TagComponent, CameraComponent, TransformComponent>();

		ASSERT(view.size_hint() > 0, "Must have at least one camera");

		for (auto entity : view)
		{
			auto& tc = view.get<TransformComponent>(entity);
			m_Camera.Set(tc.Position, tc.Size);
		}

		m_Renderer->RenderEntities(entityManager, m_Camera);

		// Render ImGui
		if (callback)
			callback();

		m_Renderer->EndScene();
	}
}
