#include "../../pch.h"

#include "ObstacleController.h"

#include "../Entities/Components.h"
#include "../Entities/Constants.h"

namespace Game
{
	void ObstacleController::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
	{
		m_Context = entityManager;
		
		auto texture = tsEngine::AssetManager::GetAsset<tsEngine::Texture>("pipe");

		float randomPosition = 0.0f;

		for (int i = 0; i < m_NumberOfObstacles - 1; i++)
		{
			randomPosition = tsEngine::Random::Ranged(150, 550);

			auto lowerPipe = entityManager->CreateEntity("lowerPipe" + std::to_string(i));
			entityManager->AddComponent<ObstacleComponent>(lowerPipe, 150.0f);
			entityManager->AddComponent<tsEngine::TransformComponent>(lowerPipe, 500.0f * i, randomPosition, m_ObstacleWidth, m_ObstacleHeight);
			entityManager->AddComponent<tsEngine::CollisionComponent>(lowerPipe, m_ObstacleWidth, m_ObstacleHeight);
			entityManager->AddComponent<tsEngine::MoverComponent>(lowerPipe);
			entityManager->AddComponent<tsEngine::SpriteComponent>(lowerPipe).Image = texture;

			auto upperPipe = entityManager->CreateEntity("upperPipe" + std::to_string(i));
			entityManager->AddComponent<ObstacleComponent>(upperPipe, 150.0f);
			entityManager->AddComponent<tsEngine::TransformComponent>(upperPipe, 500.0f * i, randomPosition - Constants::HEIGHT, m_ObstacleWidth, m_ObstacleHeight);
			entityManager->AddComponent<tsEngine::CollisionComponent>(upperPipe, m_ObstacleWidth, m_ObstacleHeight);
			entityManager->AddComponent<tsEngine::MoverComponent>(upperPipe);
			auto& spriteComponent = entityManager->AddComponent<tsEngine::SpriteComponent>(upperPipe);

			spriteComponent.Image = texture;
			spriteComponent.FlipVertical = true;
		}

		for (int i = 0; i < m_NumberOfObstacles - 1; i++)
		{
			auto scoreColider = entityManager->CreateEntity("scoreCollider" + std::to_string(i));

			entityManager->AddComponent<ScoreColliderComponent>(scoreColider);
			entityManager->AddComponent<tsEngine::TransformComponent>(scoreColider, 500.0f * i, 0.0f, 0.0f, Constants::HEIGHT);
			entityManager->AddComponent<tsEngine::CollisionComponent>(scoreColider, 5.0f, Constants::HEIGHT);
			entityManager->AddComponent<tsEngine::MoverComponent>(scoreColider);
			entityManager->AddComponent<tsEngine::SpriteComponent>(scoreColider).Image = texture;
		}
	}

	void ObstacleController::OnUpdate(float ts)
	{
		auto entityManager = m_Context;

		float randomPosition = 0.0f;

		auto view = entityManager->GetAllEntitiesWith<ObstacleComponent, tsEngine::MoverComponent, tsEngine::TransformComponent>();

		for (auto obstacle : view)
		{
			auto& move = view.get<tsEngine::MoverComponent>(obstacle);
			auto speed = view.get<ObstacleComponent>(obstacle).Speed;

			move.TranslationSpeed.x = -1.0f * speed;

			auto& transform = view.get<tsEngine::TransformComponent>(obstacle);

			if (transform.Position.x < -800.0f)
			{
				transform.Position.x = 800.0f;

				if ((uint32_t)obstacle % 2 == 1)
				{
					randomPosition = tsEngine::Random::Ranged(150, 550);
				}
				else
				{
					randomPosition = randomPosition - Constants::HEIGHT;
				}
				
				transform.Position.y = randomPosition;
			}
		}

		auto view2 = entityManager->GetAllEntitiesWith<ScoreColliderComponent, tsEngine::MoverComponent, tsEngine::TransformComponent>();

		for (auto colider : view2)
		{
			auto& move = view2.get<tsEngine::MoverComponent>(colider);
			auto speed = view2.get<ScoreColliderComponent>(colider).Speed;
			
			move.TranslationSpeed.x = -1.0f * speed;

			auto& transform = view2.get<tsEngine::TransformComponent>(colider);

			if (transform.Position.x < -800.0f)
			{
				transform.Position.x = 800.0f;
				view2.get<ScoreColliderComponent>(colider).Triggered = false;
			}
		}
	}

	void ObstacleController::Reset()
	{
		for (int i = 0; i < m_NumberOfObstacles - 1; i++)
		{
			m_Context->DestroyEntityByTag("lowerPipe" + std::to_string(i));
			m_Context->DestroyEntityByTag("upperPipe" + std::to_string(i));
			m_Context->DestroyEntityByTag("scoreCollider" + std::to_string(i));
		}

		OnCreate(m_Context);
	}
}