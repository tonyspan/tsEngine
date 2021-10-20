#include "../../pch.h"

#include "Level.h"

#include "../Entities/Components.h"
#include "../Entities/Constants.h"

namespace Game
{
    void Level::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, tsEngine::Texture* texture)
    {
        ASSERT(entityManager != nullptr, "Must pass valid pointer to entityManager to Level::OnCreate()");
        ASSERT(texture != nullptr, "Must pass valid pointer to texture to Level::Init()");

        auto wallD = entityManager->CreateEntity();

        entityManager->AddComponent<WallComponent>(wallD);
        entityManager->AddComponent<tsEngine::TransformComponent>(wallD, 0.0f, 355.0f, 1300.0f, 30.0f);
        entityManager->AddComponent<tsEngine::CollisionComponent>(wallD, 1300.0f, 30.0f);

        auto back = entityManager->CreateEntity();

        entityManager->AddComponent<tsEngine::TransformComponent>(back, 0.0f, 0.0f, Constants::WIDTH, Constants::HEIGHT);
        entityManager->AddComponent<tsEngine::SpriteComponent>(back);
        entityManager->GetComponent<tsEngine::SpriteComponent>(back).Image = texture;

        auto wallU = entityManager->CreateEntity();
        
        entityManager->AddComponent<WallComponent>(wallU);
        entityManager->AddComponent<tsEngine::TransformComponent>(wallU, 0.0f, -355.0f, 1300.0f, 30.0f);
        entityManager->AddComponent<tsEngine::CollisionComponent>(wallU, 1300.0f, 30.0f);
    }
}