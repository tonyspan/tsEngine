#include "pch.h"

#include "Application.h"

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

namespace tsEngine
{
    bool Application::Init()
    {
        LOG_INFO("Initializing application");

        ClientDefWindowData();

        // Initialize subsystems
        m_RenderManager = CreateRef<RenderManager>(m_WindowData);

        m_Window = m_RenderManager->GetRenderer()->GetWindow();
        m_EventHandler = CreateRef<EventHandler>(m_Window);
        m_EventHandler->SetEventCallback(ENGINE_BIND_FUNC(OnEvent));

        m_EntityManager = CreateRef<EntityManager>();

        m_PhysicsManager = CreateRef<PhysicsManager>();

        m_AudioManager = CreateRef<AudioManager>();

        if (ClientDefInit() != true)
        {
            LOG_CRITICAL("Error initializing sandbox systems");
            return false;
        }

        OnInitScripts();
        OnInitImGui();

        s_Instance = this;

        Random::Init();

        return true;
    }

    bool Application::Shutdown()
    {
        LOG_INFO("Shutting down application");

        ClientDefShutdown();
        OnDestroyScripts();
        OnDestroyImGui();

        return true;
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::UnloadScript(entt::entity entity)
    {
        auto& nsc = m_EntityManager->GetComponent<NativeScriptComponent>(entity);
        
        if (nsc.Instance)
        {
            nsc.Instance->OnDestroy();
            nsc.Instance->RegistryRef = nullptr;
            nsc.DestroyScript(&nsc);
        }

        m_EntityManager->RemoveComponent<NativeScriptComponent>(entity);
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
            OnUpdateScripts(ts);
            OnUpdateSystems(ts);

            previousFrameTime = frameTime;
        }

        return 0;
    }

    void Application::OnUpdateSystems(Timestep ts)
    {
        // Update all subsystems
        m_EventHandler->OnUpdate();
        OnUpdateImGui();
        m_PhysicsManager->OnUpdate(ts, m_EntityManager);
        m_RenderManager->OnUpdate(ts, m_EntityManager);

        ClientDefOnUpdate(ts);
    }

    void Application::OnInitScripts()
    {
        m_EntityManager->GetAllEntitiesWith<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
        {
            if (!nsc.Instance)
            {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->EntityRef = entity;
                nsc.Instance->RegistryRef = &m_EntityManager->GetRegistry();
                nsc.Instance->OnCreate();
            }
        });
    }

    void Application::OnUpdateScripts(Timestep ts)
    {
        m_EntityManager->GetAllEntitiesWith<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
        {
            if (!nsc.Instance)
            {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->EntityRef = entity;
                nsc.Instance->RegistryRef = &m_EntityManager->GetRegistry();
                nsc.Instance->OnCreate();
            }

            nsc.Instance->OnUpdate(ts);
        });
    }

    void Application::OnDestroyScripts()
    {
        m_EntityManager->GetAllEntitiesWith<NativeScriptComponent>().each([](auto entity, NativeScriptComponent& nsc)
        {
            if (nsc.Instance)
            {
                nsc.Instance->OnDestroy();
                nsc.Instance->RegistryRef = nullptr;
                nsc.DestroyScript(&nsc);
            }
        });
    }

    void Application::OnInitImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForSDLRenderer(m_Window->GetNativeWindow());
        ImGui_ImplSDLRenderer_Init(m_RenderManager->GetRenderer()->GetNativeRenderer());
    }

    void Application::OnUpdateImGui()
    {
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_Window->GetNativeWindow());
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));

        ClientDefOnImGuiRender();
    }

    void Application::OnDestroyImGui()
    {
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
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
    }
}
