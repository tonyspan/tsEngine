#include "../pch.h"

#include "SandboxApp.h"

#include "../AngryBird/Entities/Components.h"

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
	m_RenderManager->GetRenderer()->SetBackgroundColor({ 0, 0, 0, 1 });

	m_AudioManager->LoadSound("death", Game::Constants::DEATH);
	m_AudioManager->LoadMusic(Game::Constants::MUSIC);

	auto birdTex = tsEngine::Texture::Create(m_RenderManager->GetRenderer(), Game::Constants::PLAYER);
	auto bgTex = tsEngine::Texture::Create(m_RenderManager->GetRenderer(), Game::Constants::BG);
	auto pipeTex = tsEngine::Texture::Create(m_RenderManager->GetRenderer(), Game::Constants::PIPE);
	auto menuTex = tsEngine::Texture::Create(m_RenderManager->GetRenderer(), Game::Constants::MAIN);

	tsEngine::AssetManager::AddTexture("bird", birdTex);
	tsEngine::AssetManager::AddTexture("background", bgTex);
	tsEngine::AssetManager::AddTexture("pipe", pipeTex);
	tsEngine::AssetManager::AddTexture("menu", menuTex);

	auto font = tsEngine::Font::Create(Game::Constants::FONT, 20);
	tsEngine::AssetManager::AddFont("font", font);

	m_CameraController.OnCreate(m_EntityManager);

	m_PlayerController.OnCreate(m_EntityManager);

	m_ObstacleController.OnCreate(m_EntityManager);

	m_Level.OnCreate(m_EntityManager);

	m_MainMenu.OnCreate(tsEngine::AssetManager::GetTexture("menu"));

	m_EndMenu.OnCreate(tsEngine::AssetManager::GetTexture("menu"));

	tsEngine::StateMachine<Game::Constants::GameState>::InitState(Game::Constants::GameState::MainMenu);

	return true;
}

void Sandbox::ClientDefOnUpdate(float ts)
{
	switch (tsEngine::StateMachine<Game::Constants::GameState>::GetState())
	{
		case Game::Constants::GameState::MainMenu:
		{
			m_MainMenu.OnUpdate(ts);
			break;
		}
		case Game::Constants::GameState::EndMenu:
		{
			m_EndMenu.OnUpdate(ts);
			break;
		}
		case Game::Constants::GameState::Play:
		{
			// Reset camera position to show game screen
			// TODO: Improve it
			{
				auto e = m_EntityManager->FindEntityByTag("MainCamera");
				auto& tc = m_EntityManager->GetComponent<tsEngine::TransformComponent>(e);

				tc.Position = { 0.0f, 0.0f };
				tc.Size = { Game::Constants::WIDTH, Game::Constants::HEIGHT };
			}

			auto player = m_EntityManager->FindEntityByTag("Player");
			auto& pc = m_EntityManager->GetComponent<Game::PlayerComponent>(player);
			auto& health = m_EntityManager->GetComponent<Game::HealthComponent>(player);

			if (pc.StillPlaying)
			{
				m_PlayerController.OnUpdate(ts);
				m_ObstacleController.OnUpdate(ts);
			}
			else
			{
				pc.StillPlaying = true;
				health.Lives--;
				m_PlayerController.Reset();
				m_ObstacleController.Reset();
				m_Level.Reset();

				if (!health.Lives)
					tsEngine::StateMachine<Game::Constants::GameState>::TransitionTo(Game::Constants::GameState::EndMenu);
			}
			break;
		}
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

tsEngine::Application* tsEngine::CreateApplication()
{
	return new Sandbox();
}