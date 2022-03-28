#pragma once

#include "pch.h"

#include "Engine/Core/Base.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Log/Log.h"

#include "Engine/Window/Window.h"

namespace tsEngine
{
	class EventHandler;
	struct Event;
	struct QuitEvent;
	struct ResizeEvent;

	class AudioManager;
	class EntityManager;
	class RenderManager;
	class PhysicsManager;

	class MyImGui;
	class NativeScripting;

	class Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		bool Init();
		int Run();
		bool Shutdown();

		static Application& Get();

		Window* GetWindow();

	private:
		struct Stats;
	public:
		const Stats& GetStats() const { return m_Stats; }

		void Close();

		Ref<MyImGui> GetImGui();
		
		Ref<NativeScripting> GetScripting();
	protected:
		// To be overridden by the client
		virtual void ClientDefWindowData() = 0;
		virtual bool ClientDefInit() = 0;
		virtual void ClientDefOnUpdate(float ts) = 0;
		virtual bool ClientDefShutdown() = 0;
		virtual void ClientDefOnEvent(Event& event) {}
		virtual void ClientDefOnImGuiRender() {}
	protected:
		// Client has access to these managers/systems
		Ref<RenderManager> m_RenderManager;
		Ref<EntityManager> m_EntityManager;
		Ref<AudioManager> m_AudioManager;

		WindowProps m_WindowData;
	private:
		Application(const Application& other) = delete;
		Application& operator=(Application& other) = delete;

		void OnClose(const QuitEvent& event);
		void OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);

		void OnUpdateSystems(Timestep ts);

		void OnUpdateImGui();
	private:
		Scope<Window> m_Window;

		bool m_Running = false;

		Ref<PhysicsManager> m_PhysicsManager;
		Ref<EventHandler> m_EventHandler;

		static inline Application* s_Instance;

		// NOTE: More to be added (eventually)
		struct Stats
		{
			float FPS;
			float Timestep;
		};

		Stats m_Stats;

		// Should this be here?
		friend class MenuBase;

		friend class MyImGui;
		Ref<MyImGui> m_ImGui;

		Ref<NativeScripting> m_Script;
	};

	// To be defined in the client
	Application* CreateApplication();
}

