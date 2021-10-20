#include "../pch.h"

#include "SandboxApp.h"

#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

void Sandbox::ClientDefWindowData()
{
    m_WindowData.Title = Game::Constants::TITLE;
    m_WindowData.Width = Game::Constants::WIDTH;
    m_WindowData.Height = Game::Constants::HEIGHT;
    m_WindowData.Icon = Game::Constants::ICONPATH;
    m_WindowData.VSync = true;
}

bool Sandbox::ClientDefInit()
{
    m_RenderManager->SetBackgroundColor({ 0, 0, 0, 1 });

    m_AudioManager->LoadSound("death", Game::Constants::DEATH);
    m_AudioManager->LoadMusic(Game::Constants::MUSIC);

    tsEngine::AssetManager::CreateTexture(m_RenderManager->GetRenderer(), "bird", Game::Constants::PLAYER);
    tsEngine::AssetManager::CreateTexture(m_RenderManager->GetRenderer(), "background", Game::Constants::BG);
    tsEngine::AssetManager::CreateTexture(m_RenderManager->GetRenderer(), "pipe", Game::Constants::PIPE);
    tsEngine::AssetManager::CreateTexture(m_RenderManager->GetRenderer(), "menu", Game::Constants::MAIN);

    tsEngine::AssetManager::AddFont(Game::Constants::FONT, 20);
    tsEngine::AssetManager::LoadFont(m_RenderManager->GetRenderer(), Game::Constants::FONT);

    m_CameraController.OnCreate(m_EntityManager);

    m_PlayerController.OnCreate(m_EntityManager, tsEngine::AssetManager::GetTexture("bird"));

    m_ObstacleController.OnCreate(m_EntityManager, tsEngine::AssetManager::GetTexture("pipe"));

    m_Level.OnCreate(m_EntityManager, tsEngine::AssetManager::GetTexture("background"));

    m_MainMenu.OnCreate(m_EntityManager, tsEngine::AssetManager::GetTexture("menu"));

    m_EndMenu.OnCreate(m_EntityManager, tsEngine::AssetManager::GetTexture("menu"));

    m_AudioManager->PlayMusic();
    
    return true;
}

void Sandbox::ClientDefOnUpdate(float ts)
{
    auto& camera = m_RenderManager->GetCamera();
    
    if (!Game::MainMenu::s_HideMenu)
    {
        m_MainMenu.OnUpdate(ts, camera, m_AudioManager);

        m_RenderManager->UI_Text("Press LMB to start", 4, { m_WindowData.Width / 2, m_WindowData.Height / 2 }, { 255, 255, 255, 0 });

        return;
    }
    
    if (!Game::EndMenu::s_HideMenu)
    {
        m_EndMenu.OnUpdate(ts, camera, m_AudioManager);

        m_RenderManager->UI_Text("Game Over", 4, { m_WindowData.Width / 2, m_WindowData.Height / 2 }, { 255, 255, 255, 0 });

        return;
    }
    
    if (Game::PlayerController::s_StillPlaying)
    {
        m_PlayerController.OnUpdate(ts, m_EntityManager);
        m_CameraController.OnUpdate(ts, m_EntityManager);
        m_ObstacleController.OnUpdate(ts, m_EntityManager);

        m_MainMenu.OnUpdate(ts, camera, m_AudioManager);
        m_AudioManager->PauseMusic();

        m_RenderManager->UI_Text(std::to_string(m_PlayerController.GetPlayerScore()), 4, { m_WindowData.Width / 2, m_WindowData.Height / 8 }, { 255, 0, 0, 0 });
    }
    else
    {
        Game::EndMenu::s_HideMenu = false;
    }
}

bool Sandbox::ClientDefShutdown()
{
    return true;
}

void Sandbox::ClientDefOnEvent(tsEngine::Event& event)
{
    tsEngine::EventDispatcher dispatcher(&event);

    dispatcher.Dispatch<tsEngine::KeyboardEvent>(ENGINE_BIND_FUNC(OnKeyPressedEvent));
    dispatcher.Dispatch<tsEngine::MouseButtonEvent>(ENGINE_BIND_FUNC(OnMousePressedEvent));
    dispatcher.Dispatch<tsEngine::MousePositionEvent>(ENGINE_BIND_FUNC(OnMouseMoveEvent));
}

void Sandbox::OnKeyPressedEvent(tsEngine::KeyboardEvent& event)
{
    m_PlayerController.OnEvent(event);
}

void Sandbox::OnMousePressedEvent(tsEngine::MouseButtonEvent& event)
{
    m_PlayerController.OnEvent(event);
    m_MainMenu.OnEvent(event);
}

void Sandbox::OnMouseMoveEvent(tsEngine::MousePositionEvent& event)
{
    m_PlayerController.OnEvent(event);
}