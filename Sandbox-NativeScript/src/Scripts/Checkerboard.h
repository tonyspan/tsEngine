#pragma once

#include "Engine.h"

#include "Constants.h"

class Checkerboard : public tsEngine::ScriptableEntity
{
public:
    virtual void OnCreate() override
    {
        AddComponent<tsEngine::TransformComponent>(0.0f, 0.0f, Constants::WIDTH, Constants::HEIGHT);
        AddComponent<tsEngine::SpriteComponent>();
        GetComponent<tsEngine::SpriteComponent>().Image = tsEngine::AssetManager::GetAsset<tsEngine::Texture>("checkerboard");
    }

    virtual void OnUpdate(float ts) override
    {
    }

    virtual void OnDestroy() override
    {
    }
};