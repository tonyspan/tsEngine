#pragma once

#include "Engine/Core/Base.h"

//#include "Engine/Window/Window.h"

#include "Engine/ECS/Components.h"

#include "Engine/Utils/Utils.h"

#include <glm/glm.hpp>

#include <SDL_ttf.h>

struct SDL_Renderer;

namespace tsEngine
{
    struct WindowProps;
    class Window;
    class EntityManager;

    struct CameraData
    {
        TransformComponent* Transform = nullptr;
        CameraComponent* Camera = nullptr;

        CameraData() = default;

        operator bool() const { return (Transform != nullptr) || (Camera != nullptr); }

        void Reset()
        {
            Transform = nullptr;
            Camera = nullptr;
        }

        void Set(TransformComponent& tc, CameraComponent& cc)
        {
            Transform = &tc;
            Camera = &cc;
        }
    };

    class Renderer
    {
    public:
        Renderer(const WindowProps& props);
        ~Renderer();

        void BeginScene() const;
        void EndScene() const;

        void RenderEntities(const Ref<EntityManager>& entityManager, const CameraData& camera);

        // UI related
        void SubmitUITextForRender(const std::string& text, int MultiplierFactor, const glm::vec2& pos, const glm::u8vec4& textColor);
        void RenderUIText();

        void RenderImGui();

        SDL_Renderer* GetNativeRenderer() const;

        Window* GetWindow();

        void OpenFont(const std::string& fontPath, int fontSize);

        void SetViewport(SDL_Rect& viewport);

        void SetBackgroundColor(const glm::u8vec4& color);

        static glm::vec2 GetScreenPosition(const glm::vec2& targetPosition, const CameraData& camera);
    private:
        bool IsInsideScreen(const glm::vec2& targetPosition, const glm::vec2& targetSize, const CameraData& camera);

        // UI related
        // NOTE: UITextData struct and TextComponent are essentially the same
        // TODO: Better names for functions
        struct UITextData;
        void RenderSubmittedUIText(std::vector<UITextData>& textData);
        void RenderText(const UITextData& textData, bool showBorder = false);

        // Followed the SDL_RenderDrawXXXX convention
        void RenderDrawCircle(SDL_Renderer* renderer, int centerX, int centerY, float radius);
        void RenderDrawCircleFilled(SDL_Renderer* renderer, int centerX, int centerY, float radius, const glm::u8vec4& color);
    private:
        Scope<Window> m_Window;
        int m_WindowWidth;
        int m_WindowHeight;

        struct UITextData
        {
            std::string Text;
            int MultiplierFactor = 1;
            glm::vec2 Position;
            SDL_Color Color{};
        };

        std::vector<UITextData> m_UITextToBeRendered;

        glm::u8vec4 m_BackgroundColor;

        Scope2<TTF_Font, TTF_CloseFont> m_Font;
        int m_FontWidth;
        int m_FontHeight;

        Scope2<SDL_Renderer, SDL_DestroyRenderer> m_NativeRenderer;
    };
}
