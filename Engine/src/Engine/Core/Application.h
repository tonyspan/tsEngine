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

	class EntityManager;
	class RenderSystem;
	class PhysicsSystem;

	class ImGuiContext;
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

		void Close() { m_Running = false; }

		Ref<ImGuiContext> GetImGui() { return m_ImGui; }
	protected:
		// To be overridden by the client
		virtual void ClientDefWindowData() = 0;
		virtual bool ClientDefInit() = 0;
		virtual void ClientDefOnUpdate(float ts) = 0;
		virtual bool ClientDefShutdown() = 0;
		virtual void ClientDefOnEvent(Event& event) {}
		virtual void ClientDefOnImGuiRender() {}
	protected:
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

		Ref<RenderSystem> m_RenderManager;
		Ref<EventHandler> m_EventHandler;
		Ref<PhysicsSystem> m_PhysicsManager;

		static inline Application* s_Instance;

		// NOTE: More to be added (eventually)
		struct Stats
		{
			float FPS;
			float Timestep;
		};

		Stats m_Stats;

		friend class ImGuiContext;
		Ref<ImGuiContext> m_ImGui;
	};

	// To be defined in the client
	Application* CreateApplication();
}

