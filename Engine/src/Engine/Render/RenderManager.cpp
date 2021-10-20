#include "pch.h"

#include "RenderManager.h"

#include "Engine/Log/Log.h"

#include <SDL.h>
#include <SDL_image.h>

namespace tsEngine
{
    RenderManager::RenderManager(const WindowProps& props)
    {
        LOG_INFO("Initializing RendererManager");
        
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            LOG_CRITICAL("Unable to initialize SDL. SDL error: {0}", SDL_GetError());

        if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
            LOG_CRITICAL("Unable to initialize SDL_Image");

        if (TTF_Init() == -1)
            LOG_CRITICAL("SDL_ttf could not initialize! SDL_ttf Error: {0}", TTF_GetError());

        m_Renderer = CreateScope<Renderer>(props);
    }

    RenderManager::~RenderManager()
    {
        LOG_INFO("Shutting down RenderManager");

        SDL_Quit();
        IMG_Quit();
    }

    void RenderManager::OnUpdate(float ts, const Ref<EntityManager>& entityManager)
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
        m_Renderer->RenderUIText();
        m_Renderer->RenderImGui();

        m_Renderer->EndScene();
    }

    Renderer* RenderManager::GetRenderer()
    {
        return m_Renderer.get();
    }

    CameraData& RenderManager::GetCamera()
    {
        return m_Camera;
    }

    void RenderManager::SetBackgroundColor(const glm::u8vec4& color)
    {
        m_Renderer->SetBackgroundColor(color);
    }

    void RenderManager::UI_Text(const std::string& text, int textMultiplierFactor, const glm::vec2& pos, const glm::u8vec4& textColor)
    {
        m_Renderer->SubmitUITextForRender(text, textMultiplierFactor, pos, textColor);
    }

}
