#pragma once

#include "Engine/Window/Window.h"

#include "Engine/Render/Renderer.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"

#include <glm/glm.hpp>

//#include "imgui.h"
//#include "imgui_impl_sdl.h"
//#include "imgui_impl_sdlrenderer.h"

namespace tsEngine
{
    class RenderManager
    {
    public:
        RenderManager(const WindowProps& props = WindowProps());
        ~RenderManager();

        void OnUpdate(float ts, const Ref<EntityManager>& entityManager);
        
        Renderer* GetRenderer();

        CameraData& GetCamera();

        void SetBackgroundColor(const glm::u8vec4& color);
        
        // UI related
        void UI_Text(const std::string& text, int textMultiplierFactor, const glm::vec2& pos, const glm::u8vec4& textColor);

    private:
        RenderManager(const RenderManager& other) = delete;
        RenderManager& operator=(RenderManager& other) = delete;
    private:
        Scope<Renderer> m_Renderer;

        CameraData m_Camera;

        friend class HierarchyPanel;
    };
}
