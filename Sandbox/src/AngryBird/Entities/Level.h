#pragma once

#include "Engine.h"

namespace Game
{
    class Level
    {
    public:
        void OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager, tsEngine::Texture* texture);
    };
}