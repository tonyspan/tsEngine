#pragma once

#include "Engine.h"

namespace Game
{
    class CameraController
    {
    public:
        void OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager);
        void OnUpdate(float ts, const tsEngine::Ref<tsEngine::EntityManager>& entityManager);
    };
}
