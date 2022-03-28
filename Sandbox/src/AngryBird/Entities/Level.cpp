#include "../../pch.h"

#include "Level.h"

#include "../Entities/Components.h"
#include "../Entities/Constants.h"

namespace Game
{
    void Level::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
    {
        m_Context = entityManager;

        auto wallD = entityManager->CreateEntity("wallDown");

        entityManager->AddComponent<WallComponent>(wallD);
        entityManager->AddComponent<tsEngine::TransformComponent>(wallD, 0.0f, 355.0f, 1300.0f, 30.0f);
        entityManager->AddComponent<tsEngine::CollisionComponent>(wallD, 1300.0f, 30.0f);

        auto back = entityManager->CreateEntity("back");

        entityManager->AddComponent<tsEngine::TransformComponent>(back, 0.0f, 0.0f, Constants::WIDTH, Constants::HEIGHT);
        entityManager->AddComponent<tsEngine::SpriteComponent>(back).Image = tsEngine::AssetManager::GetAsset<tsEngine::Texture>("background");

        auto wallU = entityManager->CreateEntity("wallUp");
        
        entityManager->AddComponent<WallComponent>(wallU);
        entityManager->AddComponent<tsEngine::TransformComponent>(wallU, 0.0f, -355.0f, 1300.0f, 30.0f);
        entityManager->AddComponent<tsEngine::CollisionComponent>(wallU, 1300.0f, 30.0f);
    }

    void Level::Reset()
    {
        m_Context->DestroyEntityByTag("wallDown");
        m_Context->DestroyEntityByTag("back");
        m_Context->DestroyEntityByTag("wallUp");

        OnCreate(m_Context);
    }
}