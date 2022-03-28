#pragma once

#include "Engine/Core/Base.h"

namespace tsEngine
{
	class Window;
	class RenderSystem;

	class ImGuiContext
	{
		using ImGuiCallback = std::function<void()>;
	public:
		void Init(Window* window, const Ref<RenderSystem>& renderSystem);
		void Shutdown();

		void Begin();
		void OnImGuiRender(const ImGuiCallback& callback);
		void End();

		// NOTE: Probably shouldn't be this way
		// Ref: https://github.com/ocornut/imgui/issues/4593
		void HandleEvents(bool flag);
	private:
		Window* m_Window;
	};
}