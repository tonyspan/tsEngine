#pragma once

#include "Engine/Core/Base.h"

#include "Camera.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	class EntityManager;

	class Window;

	class Renderer;

	class RenderSystem
	{
		using ImGuiCallback = std::function<void()>;
	public:
		RenderSystem(Window* window);
		~RenderSystem();

		void OnUpdate(const Ref<EntityManager>& entityManager, const ImGuiCallback& callback = {});

		Renderer* GetRenderer() const { return m_Renderer.get(); }
		static RenderSystem& Get() { return *s_Instance; }

		const CameraData& GetCamera() const { return m_Camera; }
	private:
		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem& other) = delete;
	private:
		Scope<Renderer> m_Renderer;

		static inline RenderSystem* s_Instance;

		CameraData m_Camera;
	};
}
