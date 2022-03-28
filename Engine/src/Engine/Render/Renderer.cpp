#include "pch.h"

#include "Renderer.h"
#include "Camera.h"
#include "Texture.h"
#include "Font.h"

#include "Engine/Log/Log.h"

#include "Engine/Window/Window.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"

#include "Engine/Asset/AssetManager.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace tsEngine
{
	void Deleter2<SDL_Renderer>::operator()(SDL_Renderer* ptr)
	{
		SDL_DestroyRenderer(ptr);
	}

	Renderer::Renderer(Window* window)
		: m_Window(window)
	{
		LOG_INFO("Initializing Renderer");

		s_Instance = this;

		Uint32 flags = m_Window->IsVSync() ? SDL_RENDERER_PRESENTVSYNC : 0 | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
		m_NativeRenderer.reset(SDL_CreateRenderer(static_cast<SDL_Window*>(m_Window->GetNativeWindow()), -1, flags));

		if (!m_NativeRenderer)
			LOG_CRITICAL("Unable to create a renderer. SDL error: {0}", SDL_GetError());

		SetBackgroundColor({ 0, 0, 0, SDL_ALPHA_OPAQUE });

		m_RenderTargetTexture = Asset::Create<Texture>(m_Window->GetWidth(), m_Window->GetHeight());

		m_Stats.RendererInfo = CreateScope<SDL_RendererInfo>();
		SDL_GetRendererInfo(m_NativeRenderer.get(), m_Stats.RendererInfo.get());
	}

	Renderer::~Renderer()
	{
		LOG_INFO("Shutting down Renderer");
	}

	glm::vec2 Renderer::GetScreenPosition(const glm::vec2& targetPosition, const CameraData& camera)
	{
		glm::vec2 screenCenter = camera.GetSize() / 2.0f;

		return targetPosition - camera.GetPosition() + screenCenter;
	}

	bool Renderer::IsInsideScreen(const glm::vec2& targetPosition, const glm::vec2& targetSize, const CameraData& camera)
	{
		glm::vec2 screenPosition = GetScreenPosition(targetPosition, camera);

		return (screenPosition.x + targetSize.x / 2.0f >= 0 && screenPosition.x - targetSize.x / 2.0f <= camera.GetSize().x)
			&& (screenPosition.y + targetSize.y / 2.0f >= 0 && screenPosition.y - targetSize.y / 2.0f <= camera.GetSize().y);
	}

	// Ref: http://fredericgoset.ovh/mathematiques/courbes/en/bresenham_circle.html
	// Followed the SDL_RenderDrawXXXX convention
	static void RenderDrawCircle(SDL_Renderer* renderer, int centerX, int centerY, float radius)
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

	// Followed the SDL_RenderDrawXXXX convention
	static void RenderDrawCircleFilled(SDL_Renderer* renderer, int centerX, int centerY, float radius, const glm::u8vec4& color)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

		for (float dy = 1.0f; dy <= radius; dy++)
		{
			float dx = (float)glm::floor(glm::sqrt((2.0 * radius * dy) - glm::pow(dy, 2)));
			SDL_RenderDrawLine(renderer, (int)(centerX - dx), (int)(centerY + dy - radius), (int)(centerX + dx), (int)(centerY + dy - radius));
			SDL_RenderDrawLine(renderer, (int)(centerX - dx), (int)(centerY - dy + radius), (int)(centerX + dx), (int)(centerY - dy + radius));
		}
	}

	// NOTE: Origin coords for drawing objects is the center of the screen
	void Renderer::RenderEntities(const Ref<EntityManager>& entityManager, const CameraData& camera)
	{
		// Reset rendered entities each frame
		m_Stats.RenderedEntities = 0;

		// Trying something similar to offscreen rendering (?), render to texture (use on Editor's ImGui viewport)
		{
			SDL_SetRenderTarget(m_NativeRenderer.get(), m_RenderTargetTexture->As<SDL_Texture>());
			SetBackgroundColor(m_BackgroundColor);
			SDL_RenderClear(m_NativeRenderer.get());
		}

		auto view = entityManager->GetAllEntitiesWith<TransformComponent, SpriteComponent>();

		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& sprite = view.get<SpriteComponent>(entity);

			auto& size = transform.Size;

			if (IsInsideScreen(transform.Position, size, camera))
			{
				m_Stats.RenderedEntities++;

				glm::vec2 screenPosition = GetScreenPosition(transform.Position, camera);
				SDL_Rect dst = { (int)(screenPosition.x - size.x / 2), (int)(screenPosition.y - size.y / 2), (int)size.x, (int)size.y };
				SDL_RendererFlip flip = (SDL_RendererFlip)((sprite.FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) |
					(sprite.FlipVertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

				if (sprite.Image)
				{
					if (size == glm::vec2{ 0.0f, 0.0f })
						size = sprite.Image->Size();

					const auto spriteTexture = sprite.Image->As<SDL_Texture>();

					// NOTE: Animations not fully tested
					if (sprite.HasAnimation)
					{
						SDL_Rect src;
						int numFrames = sprite.AnimationFrames;
						int width = sprite.Width;
						int height = sprite.Height;

						src.x = height * (int)((SDL_GetTicks() / sprite.DelayPerFrame) % numFrames);
						src.y = width * (int)((SDL_GetTicks() / sprite.DelayPerFrame) % numFrames);;
						src.w = width;
						src.h = height;

						SDL_RenderCopyEx(m_NativeRenderer.get(), spriteTexture, &src, &dst, transform.Rotation, NULL, flip);
					}
					else
					{
						SDL_RenderCopyEx(m_NativeRenderer.get(), spriteTexture, NULL, &dst, transform.Rotation, NULL, flip);
					}

					SDL_SetTextureColorMod(spriteTexture, sprite.Color.r, sprite.Color.g, sprite.Color.b);
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
								dst = { (int)(screenPosition.x - collider.Size.x / 2),
										(int)(screenPosition.y - collider.Size.y / 2),
										(int)collider.Size.x,
										(int)collider.Size.y };
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

			if (IsInsideScreen(transform.Position, transform.Size, camera))
			{
				m_Stats.RenderedEntities++;

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

			if (IsInsideScreen(transform.Position, transform.Size, camera))
			{
				m_Stats.RenderedEntities++;

				glm::vec2 screenPosition = GetScreenPosition(transform.Position, camera);

				if (text.Text.empty())
					continue;

				if (!text.Font)
				{
					LOG_WARN("Renderer: No font found");
					continue;
				}

				SDL_Color color = { text.Color.r, text.Color.g, text.Color.b, text.Color.a };
				SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(text.Font->As<TTF_Font>(), text.Text.c_str(), color, text.WrapWidth);

				SDL_Texture* texture = SDL_CreateTextureFromSurface(m_NativeRenderer.get(), surface);

				auto& size = transform.Size;

				size.x = surface->w * text.MultiplierFactor;
				size.y = surface->h * text.MultiplierFactor;

				SDL_Rect dst = { (int)screenPosition.x - size.x / 2, (int)screenPosition.y - size.y / 2, (int)size.x, (int)size.y };

				SDL_RenderCopyEx(m_NativeRenderer.get(), texture, NULL, &dst, transform.Rotation, NULL, SDL_FLIP_NONE);

				// Border around text
				if (text.HasBorder)
				{
					SDL_SetRenderDrawColor(m_NativeRenderer.get(), 255, 255, 255, 50);
					SDL_RenderDrawPoint(m_NativeRenderer.get(), dst.x, dst.y);

					SDL_RenderDrawRect(m_NativeRenderer.get(), &dst);
					SetBackgroundColor(m_BackgroundColor);
				}

				SDL_FreeSurface(surface);
				SDL_DestroyTexture(texture);
			}
		}

		// Rendering to texture is done, copy the texture to the screen
		{
			SDL_SetRenderTarget(m_NativeRenderer.get(), NULL);
			SDL_RenderCopy(m_NativeRenderer.get(), m_RenderTargetTexture->As<SDL_Texture>(), NULL, NULL);
		}
	}

	Ref<Texture> Renderer::GetRenderTargetTexture() const
	{
		return m_RenderTargetTexture;
	}

	void Renderer::OnResize(uint32_t width, uint32_t height)
	{
		SDL_RenderSetLogicalSize(m_NativeRenderer.get(), width, height);

		m_RenderTargetTexture = Asset::Create<Texture>(width, height);
	}

	SDL_Renderer* Renderer::GetNativeRenderer() const
	{
		return m_NativeRenderer.get();
	}

	void Renderer::SetViewport(const glm::vec4& viewport)
	{
		SDL_Rect rect = { (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w };
		SDL_RenderSetViewport(m_NativeRenderer.get(), &rect);
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
