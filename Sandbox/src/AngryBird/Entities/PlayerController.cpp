#include "../../pch.h"

#include "PlayerController.h"

#include "../Entities/Components.h"

namespace Game
{
	void PlayerController::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
	{
		m_Context = entityManager;

		auto player = entityManager->CreateEntity("Player");

		entityManager->AddComponent<tsEngine::TransformComponent>(player, -400.0f, 0.0f, 70.0f, 70.0f);
		entityManager->AddComponent<tsEngine::CollisionComponent>(player, 60.0f, 60.0f);
		entityManager->AddComponent<PlayerComponent>(player, 50.0f, true);
		entityManager->AddComponent<tsEngine::MoverComponent>(player);
		entityManager->AddComponent<tsEngine::SpriteComponent>(player).Image = tsEngine::AssetManager::GetAsset<tsEngine::Texture>("bird");
		entityManager->AddComponent<HealthComponent>(player, 3);
		entityManager->AddComponent<ScoreComponent>(player).Score = 0;

		// UI related
		{
			auto ui = entityManager->CreateEntity("uiScore");
			entityManager->AddComponent<tsEngine::TransformComponent>(ui).Position = { 0.0f, -300.0f };
			auto& textScore = entityManager->AddComponent<tsEngine::TextComponent>(ui);

			textScore.Color = { 255, 0, 0, 0 };
			textScore.Font = tsEngine::AssetManager::GetAsset<tsEngine::Font>("font");
			textScore.HasBorder = false;
			textScore.MultiplierFactor = 4;

			ui = entityManager->CreateEntity("uiLives");
			entityManager->AddComponent<tsEngine::TransformComponent>(ui).Position = { -550.0f, -320.0f };
			auto& textLives = entityManager->AddComponent<tsEngine::TextComponent>(ui);

			textLives.Color = { 255, 0, 0, 0 };
			textLives.Font = tsEngine::AssetManager::GetAsset<tsEngine::Font>("font");
			textLives.HasBorder = false;
			textLives.MultiplierFactor = 2;
		}
	}

	void PlayerController::OnUpdate(float ts)
	{
		auto entityManager = m_Context;

		auto view = entityManager->GetAllEntitiesWith<PlayerComponent, tsEngine::TransformComponent, tsEngine::MoverComponent, tsEngine::CollisionComponent>();

		auto player = entityManager->FindEntityByTag("Player");
		auto& pc = entityManager->GetComponent<PlayerComponent>(player);
		auto& health = entityManager->GetComponent<HealthComponent>(player);

		for (auto entity : view)
		{
			auto& move = view.get<tsEngine::MoverComponent>(entity);
			auto speed = view.get<PlayerComponent>(entity).PanSpeed;
			auto& pos = view.get<tsEngine::TransformComponent>(entity);

			if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::Space))
			{
				pos.Position.y -= speed;
				move.TranslationSpeed.y = -10.0f * speed;
			}
			else
			{
				move.TranslationSpeed.y = 1.5f * speed;
			}

			auto collider = view.get<tsEngine::CollisionComponent>(entity);

			for (auto coliderEntity : collider.CollidedWith)
			{
				if (entityManager->HasComponent<WallComponent>(coliderEntity))
				{
					auto& mover = view.get<tsEngine::MoverComponent>(entity);
					mover.TranslationSpeed = { 0, 0 };

					pc.StillPlaying = false;
				}

				if (entityManager->HasComponent<ObstacleComponent>(coliderEntity))
				{
					auto& mover = view.get<tsEngine::MoverComponent>(entity);
					mover.TranslationSpeed = { 0, 0 };

					pc.StillPlaying = false;
				}

				if (entityManager->HasComponent<ScoreColliderComponent>(coliderEntity) && !entityManager->GetComponent<ScoreColliderComponent>(coliderEntity).Triggered)
				{
					entityManager->GetComponent<ScoreColliderComponent>(coliderEntity).Triggered = true;
					entityManager->GetComponent<ScoreComponent>(entity).Score++;
				}
			}
		}

		// UI related
		{
			auto uiScore = entityManager->FindEntityByTag("uiScore");
			auto& playerScore = entityManager->GetComponent<ScoreComponent>(player);
			entityManager->GetComponent<tsEngine::TextComponent>(uiScore).Text = std::to_string(playerScore.Score);

			auto uiLives = entityManager->FindEntityByTag("uiLives");
			entityManager->GetComponent<tsEngine::TextComponent>(uiLives).Text = "Lives: " + std::to_string(health.Lives);
		}
	}

	void PlayerController::OnEvent(tsEngine::Event& event)
	{
		tsEngine::EventDispatcher dispatcher(&event);

		dispatcher.Dispatch<tsEngine::KeyboardEvent>(ENGINE_BIND_FUNC(OnKeyPressedEvent));
		dispatcher.Dispatch<tsEngine::MouseButtonEvent>(ENGINE_BIND_FUNC(OnMousePressedEvent));
		dispatcher.Dispatch<tsEngine::MousePositionEvent>(ENGINE_BIND_FUNC(OnMouseMoveEvent));
	}

	void PlayerController::Reset()
	{
		auto player = m_Context->FindEntityByTag("Player");
		auto oldHealth = m_Context->GetComponent<HealthComponent>(player);

		m_Context->DestroyEntityByTag("Player");

		m_Context->DestroyEntityByTag("uiScore");
		m_Context->DestroyEntityByTag("uiLives");

		OnCreate(m_Context);

		player = m_Context->FindEntityByTag("Player");
		m_Context->GetComponent<HealthComponent>(player) = oldHealth;
	}

	void PlayerController::OnKeyPressedEvent(tsEngine::KeyboardEvent& event)
	{
	}

	void PlayerController::OnMousePressedEvent(tsEngine::MouseButtonEvent& event)
	{
	}

	void PlayerController::OnMouseMoveEvent(tsEngine::MousePositionEvent& event)
	{
	}
}
