#pragma once

#include "Engine/Core/Base.h"

#include "Camera.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	class EntityManager;

	class Window;

	class Renderer;

	class RenderManager
	{
		using ImGuiCallback = std::function<void()>;
	public:
		RenderManager(Window* window);
		~RenderManager();

		void OnUpdate(const Ref<EntityManager>& entityManager);
		// For ImGui
		void OnUpdate(const ImGuiCallback& callback);

		Renderer* GetRenderer() const;

		const CameraData& GetCamera() const;
	private:
		RenderManager(const RenderManager& other) = delete;
		RenderManager& operator=(RenderManager& other) = delete;

		void RenderImGui();
	private:
		Scope<Renderer> m_Renderer;

		CameraData m_Camera;

		ImGuiCallback m_ImGuiCallback;
	};
}
