#include "../../pch.h"

#include "CameraController.h"
#include "../Entities/Constants.h"

#include "Engine.h"

namespace Game
{
    void CameraController::OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
    {
        ASSERT(entityManager != nullptr, "Must pass valid pointer to entityManager to CameraController::OnCreate()");

        auto camera = entityManager->CreateEntity("MainCamera");

        entityManager->AddComponent<tsEngine::TransformComponent>(camera);
        entityManager->AddComponent<tsEngine::CameraComponent>(camera, 100.0f);
        entityManager->AddComponent<tsEngine::MoverComponent>(camera);

        entityManager->GetComponent<tsEngine::TransformComponent>(camera).Size = { Constants::WIDTH, Constants::HEIGHT };
    }

    void CameraController::OnUpdate(float ts, const tsEngine::Ref<tsEngine::EntityManager>& entityManager)
    {
    }
}
