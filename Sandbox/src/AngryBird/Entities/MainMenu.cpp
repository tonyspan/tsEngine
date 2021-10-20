#include "MainMenu.h"

#include "Constants.h"
#include "Components.h"

namespace Game
{
	bool MainMenu::s_HideMenu;

	constexpr char* menuName = "main";

	void MainMenu::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, tsEngine::Texture* texure)
	{
		ASSERT(entityManager != nullptr, "Must pass valid pointer to entityManager Menu::OnCreate()");

		auto mainMenu = entityManager->CreateEntity(menuName);

		AddMenuScreen(menuName);

		entityManager->AddComponent<tsEngine::TransformComponent>(mainMenu, GetMenuScreen(menuName));
		entityManager->AddComponent<tsEngine::SpriteComponent>(mainMenu);

		entityManager->GetComponent<tsEngine::SpriteComponent>(mainMenu).Image = texure;

		s_HideMenu = false;
	}

	void MainMenu::OnUpdate(float ts, tsEngine::CameraData& camera, const tsEngine::Ref<tsEngine::AudioManager>& audioManager)
	{
		if (!s_HideMenu)
			camera.Transform->Position = GetMenuScreen(menuName);
		else
			camera.Transform->Position = GetDefaultScreen();
	}

	void MainMenu::OnEvent(tsEngine::MouseButtonEvent& event)
	{
		if (event.ButtonId == tsEngine::MouseButtonId::LMB)
		{
			s_HideMenu = true;
		}
	}
}