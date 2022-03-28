#include "pch.h"

#include "RenderManager.h"

#include "Renderer.h"
#include "Camera.h"

#include "Engine/Log/Log.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"

#include "Engine/Window/Window.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace tsEngine
{
	RenderManager::RenderManager(Window* window)
	{
		LOG_INFO("Initializing RendererManager");

		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			LOG_CRITICAL("Unable to initialize SDL. SDL error: {0}", SDL_GetError());

		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
			LOG_CRITICAL("Unable to initialize SDL_Image");

		if (TTF_Init() == -1)
			LOG_CRITICAL("SDL_ttf could not initialize! SDL_ttf Error: {0}", TTF_GetError());

		m_Renderer = CreateScope<Renderer>(window);
	}

	RenderManager::~RenderManager()
	{
		LOG_INFO("Shutting down RenderManager");

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void RenderManager::OnUpdate(const Ref<EntityManager>& entityManager)
	{
		m_Renderer->BeginScene();

		m_Camera.Reset();

		auto view = entityManager->GetAllEntitiesWith<TagComponent, CameraComponent, TransformComponent>();

		for (auto entity : view)
		{
			m_Camera.Set(view.get<TransformComponent>(entity), view.get<CameraComponent>(entity));
		}

		ASSERT(m_Camera, "Must have at least one camera");

		m_Renderer->RenderEntities(entityManager, m_Camera);
		RenderImGui();

		m_Renderer->EndScene();
	}

	void RenderManager::OnUpdate(const ImGuiCallback& callback)
	{
		m_ImGuiCallback = callback;
	}

	Renderer* RenderManager::GetRenderer() const
	{
		return m_Renderer.get();
	}

	const CameraData& RenderManager::GetCamera() const
	{
		return m_Camera;
	}

	void RenderManager::RenderImGui()
	{
		if (m_ImGuiCallback)
			m_ImGuiCallback();
	}

}
