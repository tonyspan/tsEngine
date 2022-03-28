#include "MainMenu.h"

#include "Constants.h"
#include "Components.h"

namespace Game
{
	constexpr char* MenuName = "main";
	constexpr char* UIPlayEntityName = "uiPlay";
	constexpr char* UIExitEntityName = "uiExit";

	MainMenu::MainMenu()
		: MenuBase(MenuName)
	{
	}

	void MainMenu::OnCreate(const tsEngine::Ref<tsEngine::Texture>& texture)
	{
		CreateMenu(texture);

		auto entityManager = MenuContext<tsEngine::EntityManager>();
		
		auto uiPlay = entityManager->CreateEntity(UIPlayEntityName);
		entityManager->AddComponent<tsEngine::TransformComponent>(uiPlay);
		entityManager->AddComponent<tsEngine::TextComponent>(uiPlay);
		
		auto& t = MenuContext<tsEngine::EntityManager>()->GetComponent<tsEngine::TextComponent>(uiPlay);
		t.Color = { 255, 255, 255, 0 };
		t.Font = tsEngine::AssetManager::GetFont("font");
		t.HasBorder = false;
		t.MultiplierFactor = 5;
		t.Text = "Play";
		
		entityManager->GetComponent<tsEngine::TransformComponent>(uiPlay).Position = MenuGetPosition() + glm::vec2{ 200.0f, -200.0f };

		auto uiExit = entityManager->CreateEntity(UIExitEntityName);
		entityManager->AddComponent<tsEngine::TransformComponent>(uiExit);
		entityManager->AddComponent<tsEngine::TextComponent>(uiExit);

		auto& tt = MenuContext<tsEngine::EntityManager>()->GetComponent<tsEngine::TextComponent>(uiExit);
		tt.Color = { 255, 255, 255, 0 };
		tt.Font = tsEngine::AssetManager::GetFont("font");
		tt.HasBorder = false;
		tt.MultiplierFactor = 5;
		tt.Text = "Exit";

		entityManager->GetComponent<tsEngine::TransformComponent>(uiExit).Position = MenuGetPosition() + glm::vec2{ 200.0f, -50.0f };

		// temporary ?
		{
			auto uiInfo = entityManager->CreateEntity("uiInfo");
			entityManager->AddComponent<tsEngine::TransformComponent>(uiInfo);
			entityManager->AddComponent<tsEngine::TextComponent>(uiInfo);

			auto& t3 = MenuContext<tsEngine::EntityManager>()->GetComponent<tsEngine::TextComponent>(uiInfo);
			t3.Color = { 255, 255, 255, 0 };
			t3.Font = tsEngine::AssetManager::GetFont("font");
			t3.HasBorder = false;
			t3.MultiplierFactor = 1;
			t3.Text = "Game doesn't scale if you maximize/resize it (hardcoded values)";

			entityManager->GetComponent<tsEngine::TransformComponent>(uiInfo).Position = MenuGetPosition() + glm::vec2{ -320.0f, 320.0f };
		}

		MenuContext<tsEngine::AudioManager>()->PlayMusic();
	}

	void MainMenu::OnUpdate(float ts)
	{
		ShowMenu();

		auto mousePos = tsEngine::Input::MousePosition();

		if (tsEngine::Input::IsMousePressed(tsEngine::MouseCode::LMB))
		{
			if (MenuEntityHovered(UIPlayEntityName, mousePos))
			{
				MenuContext<tsEngine::AudioManager>()->PauseMusic();
				tsEngine::StateMachine<Game::Constants::GameState>::TransitionTo(Game::Constants::GameState::Play);
			}

			if (MenuEntityHovered(UIExitEntityName, mousePos))
			{
				tsEngine::Application::Get().Close();
			}
		}
	}

	void MainMenu::OnEvent(tsEngine::Event& event)
	{
		tsEngine::EventDispatcher dispatcher(&event);

		dispatcher.Dispatch<tsEngine::KeyboardEvent>(ENGINE_BIND_FUNC(OnKeyPressedEvent));
		dispatcher.Dispatch<tsEngine::MouseButtonEvent>(ENGINE_BIND_FUNC(OnMousePressedEvent));
		dispatcher.Dispatch<tsEngine::MousePositionEvent>(ENGINE_BIND_FUNC(OnMouseMoveEvent));
	}

	void MainMenu::OnKeyPressedEvent(tsEngine::KeyboardEvent& event)
	{
	}

	void MainMenu::OnMousePressedEvent(tsEngine::MouseButtonEvent& event)
	{
	}

	void MainMenu::OnMouseMoveEvent(tsEngine::MousePositionEvent& event)
	{
	}
}