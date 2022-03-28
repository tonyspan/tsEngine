#include "../../pch.h"

#include "CameraController.h"
#include "../Entities/Constants.h"

#include "Engine.h"

namespace Game
{
    void CameraController::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
    {
        auto camera = entityManager->CreateEntity("MainCamera");

        entityManager->AddComponent<tsEngine::TransformComponent>(camera).Size = { Constants::WIDTH, Constants::HEIGHT };
        entityManager->AddComponent<tsEngine::CameraComponent>(camera, 100.0f);
        entityManager->AddComponent<tsEngine::MoverComponent>(camera);
    }
}
