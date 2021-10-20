#include "../../pch.h"

#include "PlayerController.h"

#include "../Entities/Components.h"

namespace Game
{
    bool PlayerController::s_StillPlaying = true;
    int PlayerController::s_Score = 0;

    void PlayerController::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, tsEngine::Texture* texture)
    {
        ASSERT(entityManager != nullptr, "Must pass valid pointer to entityManager to PlayerController::OnCreate()");
        ASSERT(texture != nullptr, "Must pass valid pointer to texture to PlayerController::OnCreate()");

        auto player = entityManager->CreateEntity();

        entityManager->AddComponent<tsEngine::TransformComponent>(player, -400.0f, 0.0f, 70.0f, 70.0f);
        entityManager->AddComponent<tsEngine::CollisionComponent>(player, 60.0f, 60.0f);
        entityManager->AddComponent<PlayerComponent>(player, 50.0f);
        entityManager->AddComponent<tsEngine::MoverComponent>(player);
        entityManager->AddComponent<tsEngine::SpriteComponent>(player);

        entityManager->GetComponent<tsEngine::SpriteComponent>(player).Image = texture;
    }

    void PlayerController::OnUpdate(float ts, const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
    {
        auto view = entityManager->GetAllEntitiesWith<tsEngine::MoverComponent, PlayerComponent, tsEngine::TransformComponent, tsEngine::CollisionComponent>();

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
                    
                    s_StillPlaying = false;
                }

                if (entityManager->HasComponent<ObstacleComponent>(coliderEntity))
                {
                    auto& mover = view.get<tsEngine::MoverComponent>(entity);
                    mover.TranslationSpeed = { 0, 0 };

                    s_StillPlaying = false;
                }

                if (entityManager->HasComponent<ScoreColliderComponent>(coliderEntity) && !entityManager->GetComponent<ScoreColliderComponent>(coliderEntity).Triggered)
                {
                    entityManager->GetComponent<ScoreColliderComponent>(coliderEntity).Triggered = true;
                    s_Score += 1;
                }
            }
        }
    }

    const int PlayerController::GetPlayerScore() const
    {
        return s_Score;
    }

    void PlayerController::OnEvent(tsEngine::Event& event)
    {
        tsEngine::EventDispatcher dispatcher(&event);

        dispatcher.Dispatch<tsEngine::KeyboardEvent>(ENGINE_BIND_FUNC(OnKeyPressedEvent));
        dispatcher.Dispatch<tsEngine::MouseButtonEvent>(ENGINE_BIND_FUNC(OnMousePressedEvent));
        dispatcher.Dispatch<tsEngine::MousePositionEvent>(ENGINE_BIND_FUNC(OnMouseMoveEvent));
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
