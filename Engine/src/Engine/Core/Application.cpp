#include "pch.h"

#include "Application.h"

#include "SDLContext.h"
#include "Clock.h"

#include "Engine/Events/EventHandler.h"
#include "Engine/Events/MouseEvents.h"
#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/KeyboardEvents.h"

#include "Engine/ECS/EntityManager.h"

#include "Engine/Render/RenderSystem.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Asset/AssetManager.h"

#include "Engine/Utils/Random.h"

#include "Engine/Audio/AudioSystem.h"

#include "Engine/Physics/PhysicsSystem.h"

#include "Engine/ImGui/ImGuiContext.h"

#include "Engine/Scripting/Native/NativeScript.h"

namespace tsEngine
{
	bool Application::Init()
	{
		Log::Init();
		LOG_INFO("Logger initialized!");

		SDLContext::Init();

		LOG_INFO("Initializing application");

		ClientDefWindowData();

		// Initialize subsystems
		{
			m_Window = Window::Create(m_WindowData);

			m_RenderManager = CreateRef<RenderSystem>(m_Window.get());

			m_EventHandler = CreateRef<EventHandler>(m_Window.get());
			m_EventHandler->SetEventCallback(ENGINE_BIND_FUNC(OnEvent));

			m_PhysicsManager = CreateRef<PhysicsSystem>();

			AudioSystem::Init();

			AssetManager::Init();
		}

		s_Instance = this;

		if (ClientDefInit() != true)
		{
			LOG_CRITICAL("Error initializing client systems");
			return false;
		}

		m_ImGui = CreateRef<ImGuiContext>();
		m_ImGui->Init(m_Window.get(), m_RenderManager);

		NativeScript::Init(EntityManager::GetActive());

		Random::Init();

		return true;
	}

	bool Application::Shutdown()
	{
		LOG_INFO("Shutting down application");

		ClientDefShutdown();

		m_ImGui->Shutdown();

		m_Window.reset();
		m_RenderManager.reset();
		m_EventHandler.reset();
		m_PhysicsManager.reset();

		AudioSystem::Shutdown();
		NativeScript::Shutdown();
		AssetManager::Clear();

		SDLContext::Shutdown();

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

	int Application::Run()
	{
		m_Running = true;

		auto previousFrameTime = Clock::GetPerformanceCounter();

		while (m_Running)
		{
			auto frameTime = Clock::GetPerformanceCounter();

			Timestep ts = Clock::GetPerformanceDifference(frameTime, previousFrameTime);

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

		const auto& entityManager = EntityManager::GetActive();
		m_PhysicsManager->OnUpdate(ts, entityManager);
		m_RenderManager->OnUpdate(entityManager, ENGINE_BIND_FUNC(OnUpdateImGui));

		ClientDefOnUpdate(ts);
		NativeScript::OnUpdate(ts);
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
		Close();
	}

	void Application::OnResize(const ResizeEvent& event)
	{
		m_Window->SetWidth(event.NewWidth);
		m_Window->SetHeight(event.NewHeight);

		m_RenderManager->GetRenderer()->OnResize(event.NewWidth, event.NewHeight);
	}
}
