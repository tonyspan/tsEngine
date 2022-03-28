#include "pch.h"

#include "ImGuiContext.h"

#include "Engine/Render/RenderSystem.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Window/Window.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

namespace tsEngine
{
	void ImGuiContext::Init(Window* window, const Ref<RenderSystem>& renderSystem)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		m_Window = window;

		ImGui_ImplSDL2_InitForSDLRenderer(static_cast<SDL_Window*>(m_Window->GetNativeWindow()));
		ImGui_ImplSDLRenderer_Init(renderSystem->GetRenderer()->GetNativeRenderer());
	}

	void ImGuiContext::Shutdown()
	{
		ImGui_ImplSDLRenderer_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiContext::Begin()
	{
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(m_Window->GetNativeWindow()));
		ImGui::NewFrame();
	}

	void ImGuiContext::OnImGuiRender(const ImGuiCallback& callback)
	{
		if(callback)
			callback();
	}

	void ImGuiContext::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight());

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiContext::HandleEvents(bool flag)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.WantCaptureMouse &= !flag;
		io.WantCaptureKeyboard &= !flag;
	}
}