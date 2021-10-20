#pragma once

#include "pch.h"

#include "Engine/Core/Base.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Log/Log.h"

#include "Engine/Window/Window.h"
#include "Engine/Events/EventHandler.h"

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/ScriptableEntity.h"

#include "Engine/Render/Renderer.h"
#include "Engine/Render/RenderManager.h"
#include "Engine/Render/Texture.h"

#include "Engine/Utils/AssetManager.h"
#include "Engine/Utils/Random.h"

#include "Engine/Audio/AudioManager.h"

#include "Engine/Physics/PhysicsManager.h"

namespace tsEngine
{
	static void SDLVersion()
	{
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);
		LOG_INFO("Compiled SDL version: {}.{}.{}", compiled.major, compiled.minor, compiled.patch);
		LOG_INFO("Linked SDL version: {}.{}.{}", linked.major, linked.minor, linked.patch);
	}

	class Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		bool Init();
		int Run();
		bool Shutdown();

		static Application& Get() { return *s_Instance; }
		
		// NOTE: Stats is private
		struct Stats;
		Stats& GetStats() { return m_Stats; }

		void Close();
		
		template<typename T>
		void LoadScript(entt::entity entity)
		{
			auto& nsc = m_EntityManager->AddNativeScript<NativeScriptComponent>(entity);

			nsc.Bind<T>();

			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->EntityRef = entity;
				nsc.Instance->RegistryRef = &m_EntityManager->GetRegistry();
				nsc.Instance->OnCreate();
			}
		}

		void UnloadScript(entt::entity entity);

	protected:
		// To be overridden by the client
		virtual void ClientDefWindowData() = 0;
		virtual bool ClientDefInit() = 0;
		virtual void ClientDefOnUpdate(float ts) = 0;
		virtual bool ClientDefShutdown() = 0;
		virtual void ClientDefOnEvent(Event& event) {}
		virtual void ClientDefOnImGuiRender() {}

	protected:
		Ref<RenderManager> m_RenderManager;
		Ref<PhysicsManager> m_PhysicsManager;
		Ref<EntityManager> m_EntityManager;
		Ref<AudioManager> m_AudioManager;
		Ref<EventHandler> m_EventHandler;
		
		WindowProps m_WindowData;
	private:
		Application(const Application& other) = delete;
		Application& operator=(Application& other) = delete;

		void OnClose(const QuitEvent& event);
		void OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);

		void OnUpdateSystems(Timestep ts);
		
		void OnInitScripts();
		void OnUpdateScripts(Timestep ts);
		void OnDestroyScripts();

		void OnInitImGui();
		void OnUpdateImGui();
		void OnDestroyImGui();
	private:
		Window* m_Window = nullptr;
		bool m_Running = false;

		static inline Application* s_Instance;

		// NOTE: More to be added (eventually)
		struct Stats
		{
			float FPS;
		};

		static inline Stats m_Stats;
	};

	// To be defined in the client
	extern Application* CreateApplication();
}

