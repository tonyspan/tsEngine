#pragma once

#include "Engine.h"

class BasicController : public tsEngine::ScriptableEntity
{
public:
    virtual void OnCreate() override
    {
    }

    virtual void OnUpdate(float ts) override
    {
        float speed = 1.0f;
        auto& transform = GetComponent<tsEngine::TransformComponent>();

        // Y-axis are flipped
        if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::A))
            transform.Position.x -= speed;
        if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::D))
            transform.Position.x += speed;
        if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::W))
            transform.Position.y -= speed;
        if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::S))
            transform.Position.y += speed;
    }

    virtual void OnDestroy() override
    {
    }
};