#include "pch.h"

#include "MyImGui.h"

#include "Engine/Core/Application.h"

//#include "Engine/Events/EventHandler.h"

#include "Engine/Render/RenderManager.h"
#include "Engine/Render/Renderer.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

namespace tsEngine
{
	void MyImGui::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		Application& app = Application::Get();
		ImGui_ImplSDL2_InitForSDLRenderer(static_cast<SDL_Window*>(app.GetWindow()->GetNativeWindow()));
		ImGui_ImplSDLRenderer_Init(app.m_RenderManager->GetRenderer()->GetNativeRenderer());
	}

	void MyImGui::Shutdown()
	{
		ImGui_ImplSDLRenderer_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void MyImGui::Begin()
	{
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(Application::Get().GetWindow()->GetNativeWindow()));
		ImGui::NewFrame();
	}

	void MyImGui::OnImGuiRender(const ImGuiCallback& callback)
	{
		callback();
	}

	void MyImGui::End()
	{
		Application& app = Application::Get();
		Window* window = app.GetWindow();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)window->GetWidth(), (float)window->GetHeight());

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}

	// NOTE: Probably shouldn't be this way
	// Ref: https://github.com/ocornut/imgui/issues/4593
	void MyImGui::BypassImGuiHandleEvents(bool flag)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.WantCaptureMouse &= !flag;
		io.WantCaptureKeyboard &= !flag;
	}
}