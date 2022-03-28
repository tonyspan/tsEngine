#include "pch.h"

#include "Application.h"

#include "Engine/Events/EventHandler.h"
#include "Engine/Events/MouseEvents.h"
#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/KeyboardEvents.h"

#include "Engine/ECS/EntityManager.h"

#include "Engine/Render/RenderManager.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Utils/AssetManager.h"
#include "Engine/Utils/Random.h"

#include "Engine/Audio/AudioManager.h"

#include "Engine/Physics/PhysicsManager.h"

#include "Engine/ImGui/MyImGui.h"

#include "Engine/Scripting/NativeScripting.h"

namespace tsEngine
{
	bool Application::Init()
	{
		Log::Init();
		LOG_INFO("Logger initialized!");

		LOG_INFO("Initializing application");

		ClientDefWindowData();

		// Initialize subsystems

		m_Window = Window::Create(m_WindowData);

		m_RenderManager = CreateRef<RenderManager>(m_Window.get());

		m_EventHandler = CreateRef<EventHandler>(m_Window.get());
		m_EventHandler->SetEventCallback(ENGINE_BIND_FUNC(OnEvent));

		m_EntityManager = CreateRef<EntityManager>();

		m_PhysicsManager = CreateRef<PhysicsManager>();

		m_AudioManager = CreateRef<AudioManager>();

		s_Instance = this;

		if (ClientDefInit() != true)
		{
			LOG_CRITICAL("Error initializing client systems");
			return false;
		}

		m_ImGui = CreateRef<MyImGui>();
		m_ImGui->Init();

		m_Script = CreateRef<NativeScripting>(m_EntityManager);
		m_Script->Init();

		Random::Init();

		return true;
	}

	bool Application::Shutdown()
	{
		LOG_INFO("Shutting down application");

		ClientDefShutdown();
		m_Script->Shutdown();
		m_ImGui->Shutdown();

		AssetManager::Clear();

		return true;
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	Window* Application::GetWindow()
	{
		return m_Window.get();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	Ref<MyImGui> Application::GetImGui()
	{
		return m_ImGui;
	}

	Ref<NativeScripting> Application::GetScripting()
	{
		return m_Script;
	}

	int Application::Run()
	{
		m_Running = true;
		auto previousFrameTime = SDL_GetPerformanceCounter();

		while (m_Running)
		{
			auto frameTime = SDL_GetPerformanceCounter();

			Timestep ts = (frameTime - previousFrameTime) / static_cast<float>(SDL_GetPerformanceFrequency());

			//LOG_INFO("FPS: {}", 1.0f / ts);
			m_Stats.FPS = 1.0f / ts;
			m_Stats.Timestep = ts;

			OnUpdateSystems(ts);

			previousFrameTime = frameTime;
		}

		return 0;
	}

	void Application::OnUpdateSystems(Timestep ts)
	{
		// Update all subsystems
		
		m_EventHandler->OnUpdate();

		m_PhysicsManager->OnUpdate(ts, m_EntityManager);
		m_RenderManager->OnUpdate(m_EntityManager);
		m_RenderManager->OnUpdate(ENGINE_BIND_FUNC(OnUpdateImGui));

		ClientDefOnUpdate(ts);
		m_Script->OnUpdate(ts);
	}

	void Application::OnUpdateImGui()
	{
		m_ImGui->Begin();
		m_ImGui->OnImGuiRender(ENGINE_BIND_FUNC(ClientDefOnImGuiRender));
		m_ImGui->End();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(&event);

		dispatcher.Dispatch<QuitEvent>(ENGINE_BIND_FUNC(OnClose));
		dispatcher.Dispatch<ResizeEvent>(ENGINE_BIND_FUNC(OnResize));

		ClientDefOnEvent(event);
	}

	void Application::OnClose(const QuitEvent& event)
	{
		m_Running = false;
	}

	void Application::OnResize(const ResizeEvent& event)
	{
		m_Window->SetWidth(event.NewWidth);
		m_Window->SetHeight(event.NewHeight);

		m_RenderManager->GetRenderer()->OnResize(event.NewWidth, event.NewHeight);
	}
}
