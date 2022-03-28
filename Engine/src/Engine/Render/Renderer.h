#pragma once

#include "Engine/Core/Base.h"

#include <glm/glm.hpp>

struct SDL_Renderer;
struct SDL_RendererInfo;

namespace tsEngine
{
	struct WindowProps;
	class Window;
	class EntityManager;

	class CameraData;

	class Texture;

	class Renderer
	{
	public:
		Renderer(Window* window);
		~Renderer();

		void BeginScene() const;
		void EndScene() const;

		void RenderEntities(const Ref<EntityManager>& entityManager, const CameraData& camera);

		Ref<Texture> GetRenderTargetTexture() const;

		void OnResize(uint32_t width, uint32_t height);

		SDL_Renderer* GetNativeRenderer() const;

		// x, y correspond to (x,y) coords
		// z, w correspond to width, height
		void SetViewport(const glm::vec4& viewport);

		void SetBackgroundColor(const glm::u8vec4& color);

		static glm::vec2 GetScreenPosition(const glm::vec2& targetPosition, const CameraData& camera);
	private:
		struct RenderStats;
	public:
		const RenderStats& GetStats() const { return m_Stats; }
	private:
		bool IsInsideScreen(const glm::vec2& targetPosition, const glm::vec2& targetSize, const CameraData& camera);
	private:
		Window* m_Window;

		Ref<Texture> m_RenderTargetTexture;

		glm::u8vec4 m_BackgroundColor;

		Scope2<SDL_Renderer> m_NativeRenderer;

		struct RenderStats
		{
			int RenderedEntities = 0;
			Scope<SDL_RendererInfo> RendererInfo;
		};

		RenderStats m_Stats;

		friend class Texture;
		friend class Serializer;

		// For use on Serializer and Texure
		static inline Renderer* s_Instance;
	};
}
