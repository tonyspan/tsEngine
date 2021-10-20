#include "EndMenu.h"

#include "Constants.h"
#include "Components.h"

namespace Game
{
	bool EndMenu::s_HideMenu;

	constexpr char* menuName = "end";

	void EndMenu::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, tsEngine::Texture* texure)
	{
		ASSERT(entityManager != nullptr, "Must pass valid pointer to entityManager Menu::OnCreate()");

		auto endMenu = entityManager->CreateEntity(menuName);

		AddMenuScreen(menuName);

		entityManager->AddComponent<tsEngine::TransformComponent>(endMenu, GetMenuScreen(menuName));
		entityManager->AddComponent<tsEngine::SpriteComponent>(endMenu);

		entityManager->GetComponent<tsEngine::SpriteComponent>(endMenu).Image = texure;

		s_HideMenu = true;
	}

	void EndMenu::OnUpdate(float ts, tsEngine::CameraData& camera, const tsEngine::Ref<tsEngine::AudioManager>& audioManager)
	{
		if (!s_HideMenu)
		{
			camera.Transform->Position = GetMenuScreen(menuName);

			float r = tsEngine::Random::Float() / tsEngine::Random::Float();
			
			if (r < 0.4f)
				audioManager->PlaySound("death");
		}
		else
		{
			camera.Transform->Position = GetDefaultScreen();
		}
	}

	void EndMenu::OnEvent(tsEngine::MouseButtonEvent& event)
	{
	}
}