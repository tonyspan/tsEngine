#pragma once

//#include "imgui.h"

namespace tsEngine
{
	class MyImGui
	{
		using ImGuiCallback = std::function<void()>;
	public:
		void Init();
		void Shutdown();

		void Begin();
		void OnImGuiRender(const ImGuiCallback& callback);
		void End();

		void BypassImGuiHandleEvents(bool flag);
	};
}