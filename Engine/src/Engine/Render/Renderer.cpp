#include "pch.h"

#include "Renderer.h"

#include "Engine/Log/Log.h"

#include "Engine/Window/Window.h"

#include "Engine/Render/Texture.h"

#include "Engine/ECS/EntityManager.h"
//#include "Engine/ECS/Components.h"

#include "Engine/Utils/AssetManager.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "imgui.h"
//#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

namespace tsEngine
{
    Renderer::Renderer(const WindowProps& props)
    {
        LOG_INFO("Initializing Renderer");

        m_Window = CreateScope<Window>(props);

        m_NativeRenderer.reset(SDL_CreateRenderer(m_Window->GetNativeWindow(), -1, m_Window->GetVSync() ? SDL_RENDERER_PRESENTVSYNC : 0 | SDL_RENDERER_ACCELERATED));

        if (!m_NativeRenderer)
            LOG_CRITICAL("Unable to create a renderer. SDL error: {0}", SDL_GetError());

        SDL_GetWindowSize(m_Window->GetNativeWindow(), &m_WindowWidth, &m_WindowHeight);
        SDL_RenderSetLogicalSize(m_NativeRenderer.get(), m_WindowWidth, m_WindowHeight);

        SetBackgroundColor({ 100, 150, 235, SDL_ALPHA_OPAQUE });
    }

    Renderer::~Renderer()
    {
        LOG_INFO("Shutting down Renderer");
    }

    glm::vec2 Renderer::GetScreenPosition(const glm::vec2& targetPosition, const CameraData& camera)
    {
        glm::vec2 screenCenter = camera.Transform->Size / 2.0f;

        return targetPosition - camera.Transform->Position + screenCenter;
    }

    bool Renderer::IsInsideScreen(const glm::vec2& targetPosition, const glm::vec2& targetSize, const CameraData& camera)
    {
        glm::vec2 screenPosition = GetScreenPosition(targetPosition, camera);

        return (screenPosition.x + targetSize.x / 2.0f >= 0 && screenPosition.x - targetSize.x / 2.0f <= camera.Transform->Size.x)
            && (screenPosition.y + targetSize.y / 2.0f >= 0 && screenPosition.y - targetSize.y / 2.0f <= camera.Transform->Size.y);
    }

    // Ref: http://fredericgoset.ovh/mathematiques/courbes/en/bresenham_circle.html
    void Renderer::RenderDrawCircle(SDL_Renderer* renderer, int centerX, int centerY, float radius)
    {
        int x = 0;
        int y = (int)radius;
        int error = 5 - 4 * (int)radius;

        while (x <= y)
        {
            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
            SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);

            if (error > 0)
            {
                y--;
                error -= 8 * y;
            }

            x++;

            error += 8 * x + 4;
        }
    }

    void Renderer::RenderDrawCircleFilled(SDL_Renderer* renderer, int centerX, int centerY, float radius, const glm::u8vec4& color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        for (float dy = 1.0f; dy <= radius; dy++)
        {
            float dx = (float)glm::floor(glm::sqrt((2.0 * radius * dy) - glm::pow(dy, 2)));
            SDL_RenderDrawLine(renderer, centerX - dx, centerY + dy - radius, centerX + dx, centerY + dy - radius);
            SDL_RenderDrawLine(renderer, centerX - dx, centerY - dy + radius, centerX + dx, centerY - dy + radius);
        }
    }

    // NOTE: Origin coords for drawing objects is the center of the screen
    void Renderer::RenderEntities(const Ref<EntityManager>& entityManager, const CameraData& camera)
    {
        auto view = entityManager->GetAllEntitiesWith<TransformComponent, SpriteComponent>();

        for (auto entity : view)
        {
            auto& transform = view.get<TransformComponent>(entity);
            auto& sprite = view.get<SpriteComponent>(entity);

            auto& size = transform.Size;
            if (size == glm::vec2{ 0.0f, 0.0f })
            {
                int w, h;
                SDL_QueryTexture(Texture::GetRaw(sprite.Image), NULL, NULL, &w, &h);
                size.x = (float)w;
                size.y = (float)h;
            }

            if (IsInsideScreen(transform.Position, glm::vec2(size.x, size.y), camera))
            {
                glm::vec2 screenPosition = GetScreenPosition(transform.Position, camera);
                SDL_Rect dst = { (int)(screenPosition.x - size.x / 2), (int)(screenPosition.y - size.y / 2), (int)size.x, (int)size.y };
                SDL_RendererFlip flip = (SDL_RendererFlip)((sprite.FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (sprite.FlipVertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

                if (sprite.Image)
                {
                    // NOTE: Animations not tested
                    if (sprite.Animation)
                    {
                        SDL_Rect src;
                        int numFrames = sprite.AnimationFrames;
                        int height = sprite.Height;

                        src.x = height * (int)((SDL_GetTicks() / sprite.DelayPerFrame) % numFrames);
                        src.y = 0;
                        src.w = height;
                        src.h = height;

                        SDL_RenderCopyEx(m_NativeRenderer.get(), Texture::GetRaw(sprite.Image), &src, &dst, transform.Rotation, NULL, flip);
                    }
                    else
                    {
                        SDL_RenderCopyEx(m_NativeRenderer.get(), Texture::GetRaw(sprite.Image), NULL, &dst, transform.Rotation, NULL, flip);
                    }
                    
                    SDL_SetTextureColorMod(Texture::GetRaw(sprite.Image), sprite.Color.r, sprite.Color.g, sprite.Color.b);
                }
                else
                {
                    SDL_SetRenderDrawColor(m_NativeRenderer.get(), sprite.Color.r, sprite.Color.g, sprite.Color.b, sprite.Color.a);
                    SDL_RenderFillRect(m_NativeRenderer.get(), &dst);
                }

                if (sprite.DrawPhysicsCollider)
                {
                    SDL_SetRenderDrawColor(m_NativeRenderer.get(), 255, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint(m_NativeRenderer.get(), (int)screenPosition.x, (int)screenPosition.y);

                    auto view2 = entityManager->GetAllEntitiesWith<SpriteComponent, CollisionComponent>();

                    for (auto entity : view2)
                    {
                        auto& collider = entityManager->GetComponent<CollisionComponent>(entity);

                        SDL_SetRenderDrawColor(m_NativeRenderer.get(), 255, 0, 0, SDL_ALPHA_OPAQUE);

                        // NOTE: Still here for compatibility with Sandbox project
                        {
                            if (collider.Shape == CollisionShape::AABox)
                            {
                                dst = { (int)(screenPosition.x - collider.Size.x / 2), (int)(screenPosition.y - collider.Size.y / 2), (int)collider.Size.x, (int)collider.Size.y };
                                SDL_RenderDrawRect(m_NativeRenderer.get(), &dst);
                            }
                            else if (collider.Shape == CollisionShape::Circle)
                            {
                                auto radius = 0.5f * (float)glm::sqrt(glm::pow(collider.Size.x, 2) + glm::pow(collider.Size.y, 2));
                                RenderDrawCircle(m_NativeRenderer.get(), (int)screenPosition.x, (int)screenPosition.y, radius);
                            }
                        }
                    }
                }
                SetBackgroundColor(m_BackgroundColor);
            }
        }

        auto view2 = entityManager->GetAllEntitiesWith<TransformComponent, CircleComponent>();

        for (auto entity : view2)
        {
            auto& transform = view2.get<TransformComponent>(entity);
            auto& circle = view2.get<CircleComponent>(entity);

            if (IsInsideScreen(transform.Position, glm::vec2(transform.Size.x, transform.Size.y), camera))
            {
                glm::vec2 screenPosition = GetScreenPosition(transform.Position, camera);

                auto radius = 0.5f * (float)glm::sqrt(glm::pow(transform.Size.x, 2) + glm::pow(transform.Size.y, 2));
                RenderDrawCircleFilled(m_NativeRenderer.get(), (int)screenPosition.x, (int)screenPosition.y, radius, circle.Color);

                if (circle.DrawPhysicsCollider)
                {
                    SDL_SetRenderDrawColor(m_NativeRenderer.get(), 255, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint(m_NativeRenderer.get(), (int)screenPosition.x, (int)screenPosition.y);

                    auto view3 = entityManager->GetAllEntitiesWith<CircleComponent, CollisionComponent>();

                    for (auto entity : view3)
                    {
                        auto& collider = entityManager->GetComponent<CollisionComponent>(entity);

                        SDL_SetRenderDrawColor(m_NativeRenderer.get(), 255, 0, 0, SDL_ALPHA_OPAQUE);

                        auto radius = 0.5f * (float)glm::sqrt(glm::pow(collider.Size.x, 2) + glm::pow(collider.Size.y, 2));
                        RenderDrawCircle(m_NativeRenderer.get(), (int)screenPosition.x, (int)screenPosition.y, radius);
                    }
                }
                SetBackgroundColor(m_BackgroundColor);
            }
        }

        auto view4 = entityManager->GetAllEntitiesWith<TransformComponent, TextComponent>();

        for (auto entity : view4)
        {
            auto& transform = view4.get<TransformComponent>(entity);
            auto& text = view4.get<TextComponent>(entity);
            glm::vec2 screenPosition = GetScreenPosition(transform.Position, camera);

            RenderText({ text.Text, text.MultiplierFactor, { screenPosition.x, screenPosition.y }, { text.Color.r, text.Color.g, text.Color.b, text.Color.a } }, true);
        }
    }

    void Renderer::RenderUIText()
    {
        RenderSubmittedUIText(m_UITextToBeRendered);
    }
    
    void Renderer::RenderImGui()
    {
        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }

    SDL_Renderer* Renderer::GetNativeRenderer() const
    {
        return m_NativeRenderer.get();
    }

    Window* Renderer::GetWindow()
    {
        return m_Window.get();
    }

    void Renderer::OpenFont(const std::string& fontPath, int fontSize)
    {
        m_Font.reset(TTF_OpenFont(fontPath.c_str(), fontSize));

        if (!m_Font)
            LOG_ERROR("Failed to load font. SDL_ttf Error: {0}", TTF_GetError());
    }

    void Renderer::SetViewport(SDL_Rect& viewport)
    {
        SDL_RenderSetViewport(m_NativeRenderer.get(), &viewport);
    }

    void Renderer::RenderSubmittedUIText(std::vector<UITextData>& textData)
    {
        if (textData.empty())
            return;

        for (const auto& text : textData)
        {
            RenderText(text);
        }

        textData.clear();
    }

    void Renderer::RenderText(const UITextData& textData, bool showBorder)
    {
        if (textData.Text.empty())
            return;

        if (!m_Font)
            OpenFont(AssetManager::s_DefaultFontPath.string(), 10);

        SDL_Surface* surface = TTF_RenderText_Solid(m_Font.get(), textData.Text.c_str(), textData.Color);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_NativeRenderer.get(), surface);

        if (!texture)
            LOG_ERROR("Unable to create texture. SDL Error: {}", SDL_GetError());

        auto width = surface->w * textData.MultiplierFactor;
        auto height = surface->h * textData.MultiplierFactor;
        
        // Substracting (width / 2) & (height / 2) to align the text to the middle of rectangle
        SDL_Rect dst = { (int)textData.Position.x - width / 2, (int)textData.Position.y - height / 2, width, height };

        SDL_RenderCopy(m_NativeRenderer.get(), texture, NULL, &dst);

        // Border around text
        if (showBorder)
        {
            SDL_SetRenderDrawColor(m_NativeRenderer.get(), 255, 255, 255, 50);
            SDL_RenderDrawPoint(m_NativeRenderer.get(), dst.x, dst.y);

            SDL_RenderDrawRect(m_NativeRenderer.get(), &dst);
            SetBackgroundColor(m_BackgroundColor);

            m_FontWidth = width;
            m_FontHeight = height;
        }

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void Renderer::SubmitUITextForRender(const std::string& text, int MultiplierFactor, const glm::vec2& pos, const glm::u8vec4& textColor)
    {
        m_UITextToBeRendered.emplace_back(UITextData{ text, MultiplierFactor, pos, { textColor.r, textColor.g, textColor.b, textColor.a } });
    }

    void Renderer::SetBackgroundColor(const glm::u8vec4& color)
    {
        m_BackgroundColor = color;
        SDL_SetRenderDrawColor(m_NativeRenderer.get(), m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
    }

    void Renderer::BeginScene() const
    {
        SDL_RenderClear(m_NativeRenderer.get());
    }

    void Renderer::EndScene() const
    {
        SDL_RenderPresent(m_NativeRenderer.get());
    }
}
