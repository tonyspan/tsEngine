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

		auto entityManager = tsEngine::EntityManager::GetActive();

		auto uiPlay = entityManager->CreateEntity(UIPlayEntityName);
		entityManager->AddComponent<tsEngine::TransformComponent>(uiPlay);
		auto& playText = entityManager->AddComponent<tsEngine::TextComponent>(uiPlay);

		playText.Color = { 255, 255, 255, 0 };
		playText.Font = tsEngine::AssetManager::GetAsset<tsEngine::Font>("font");
		playText.HasBorder = false;
		playText.MultiplierFactor = 5;
		playText.Text = "Play";

		entityManager->GetComponent<tsEngine::TransformComponent>(uiPlay).Position = MenuGetPosition() + glm::vec2{ 200.0f, -200.0f };

		auto uiExit = entityManager->CreateEntity(UIExitEntityName);
		entityManager->AddComponent<tsEngine::TransformComponent>(uiExit);
		auto& exitText = entityManager->AddComponent<tsEngine::TextComponent>(uiExit);

		exitText.Color = { 255, 255, 255, 0 };
		exitText.Font = tsEngine::AssetManager::GetAsset<tsEngine::Font>("font");
		exitText.HasBorder = false;
		exitText.MultiplierFactor = 5;
		exitText.Text = "Exit";

		entityManager->GetComponent<tsEngine::TransformComponent>(uiExit).Position = MenuGetPosition() + glm::vec2{ 200.0f, -50.0f };

		// temporary ?
		{
			auto uiInfo = entityManager->CreateEntity("uiInfo");
			entityManager->AddComponent<tsEngine::TransformComponent>(uiInfo);
			auto& infoText = entityManager->AddComponent<tsEngine::TextComponent>(uiInfo);

			infoText.Color = { 255, 255, 255, 0 };
			infoText.Font = tsEngine::AssetManager::GetAsset<tsEngine::Font>("font");
			infoText.HasBorder = false;
			infoText.MultiplierFactor = 1;
			infoText.Text = "Game doesn't scale if you maximize/resize it (hardcoded values)";

			entityManager->GetComponent<tsEngine::TransformComponent>(uiInfo).Position = MenuGetPosition() + glm::vec2{ -320.0f, 320.0f };
		}

		tsEngine::AudioSystem::PlayMusic();
	}

	void MainMenu::OnUpdate(float ts)
	{
		ShowMenu();

		auto mousePos = tsEngine::Input::MousePosition();

		if (tsEngine::Input::IsMousePressed(tsEngine::MouseCode::LMB))
		{
			if (MenuEntityHovered(UIPlayEntityName, mousePos))
			{
				tsEngine::AudioSystem::PauseMusic();
				tsEngine::StateMachine<Game::Constants::GameState>::GoTo(Game::Constants::GameState::Play);
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