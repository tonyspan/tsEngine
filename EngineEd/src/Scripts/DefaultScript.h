#pragma once

#include "Engine.h"

class DefaultScript : public tsEngine::ScriptableEntity
{
public:
    virtual void OnCreate() override
    {
    }

    virtual void OnUpdate(float ts) override
    {
        // For testing when switching between scripts
        if (HasComponent<tsEngine::MoverComponent>())
            RemoveComponent<tsEngine::MoverComponent>();

        if (HasComponent<tsEngine::CollisionComponent>())
            RemoveComponent<tsEngine::CollisionComponent>();
    }

    virtual void OnDestroy() override
    {
    }
};