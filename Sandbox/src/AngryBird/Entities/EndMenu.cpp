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

		auto entityManager = MenuContext<tsEngine::EntityManager>();
		auto ui = entityManager->CreateEntity(UIGOName);
		entityManager->AddComponent<tsEngine::TransformComponent>(ui);
		entityManager->AddComponent<tsEngine::TextComponent>(ui);

		auto& t = MenuContext<tsEngine::EntityManager>()->GetComponent<tsEngine::TextComponent>(ui);
		t.Color = { 255, 255, 255, 0 };
		t.Font = tsEngine::AssetManager::GetFont("font");
		t.HasBorder = false;
		t.MultiplierFactor = 5;
		t.Text = "Game Over";

		entityManager->GetComponent<tsEngine::TransformComponent>(ui).Position = MenuGetPosition();
	}

	void EndMenu::OnUpdate(float ts)
	{
		ShowMenu();

		float r = tsEngine::Random::Float() / tsEngine::Random::Float();

		if (r < 0.4f)
			MenuContext<tsEngine::AudioManager>()->PlaySound("death");
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