#include "EndMenu.h"

#include "Constants.h"
#include "Components.h"

namespace Game
{
	constexpr char* MenuName = "end";
	constexpr char* UIGOName = "uiGameOver";

	EndMenu::EndMenu()
		: MenuBase(MenuName)
	{
	}

	void EndMenu::OnCreate(const tsEngine::Ref<tsEngine::Texture>& texture)
	{
		CreateMenu(texture);

		auto entityManager = tsEngine::EntityManager::GetActive();
		auto ui = entityManager->CreateEntity(UIGOName);
		entityManager->AddComponent<tsEngine::TransformComponent>(ui).Position = MenuGetPosition();
		auto& text = entityManager->AddComponent<tsEngine::TextComponent>(ui);

		text.Color = { 255, 255, 255, 0 };
		text.Font = tsEngine::AssetManager::GetAsset<tsEngine::Font>("font");
		text.HasBorder = false;
		text.MultiplierFactor = 5;
		text.Text = "Game Over";
	}

	void EndMenu::OnUpdate(float ts)
	{
		ShowMenu();

		float r = tsEngine::Random::Float() / tsEngine::Random::Float();

		if (r < 0.4f)
			tsEngine::AudioSystem::PlaySound("death");
	}

	void EndMenu::OnEvent(tsEngine::Event& event)
	{
		tsEngine::EventDispatcher dispatcher(&event);

		dispatcher.Dispatch<tsEngine::KeyboardEvent>(ENGINE_BIND_FUNC(OnKeyPressedEvent));
		dispatcher.Dispatch<tsEngine::MouseButtonEvent>(ENGINE_BIND_FUNC(OnMousePressedEvent));
		dispatcher.Dispatch<tsEngine::MousePositionEvent>(ENGINE_BIND_FUNC(OnMouseMoveEvent));
	}

	void EndMenu::OnKeyPressedEvent(tsEngine::KeyboardEvent& event)
	{
	}

	void EndMenu::OnMousePressedEvent(tsEngine::MouseButtonEvent& event)
	{
	}

	void EndMenu::OnMouseMoveEvent(tsEngine::MousePositionEvent& event)
	{
	}
}