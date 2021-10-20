#pragma once

#include "Engine.h"

#include "Constants.h"

class CameraController : public tsEngine::ScriptableEntity
{
public:
    virtual void OnCreate() override
    {
        AddComponent<tsEngine::CameraComponent>(100.0f);
        AddComponent<tsEngine::TransformComponent>();
        AddComponent<tsEngine::MoverComponent>(0.0f, 0.0f);
        GetComponent<tsEngine::TransformComponent>().Size = { Constants::WIDTH, Constants::HEIGHT };
    }

    virtual void OnUpdate(float ts) override
    {
        float speed = 1.0f;
        auto& move = GetComponent<tsEngine::TransformComponent>();
        
        // X-axis is flipped for some reason
        if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::A))
            move.Position.x += speed;
        if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::D))
            move.Position.x -= speed;
        if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::W))
            move.Position.y += speed;
        if (tsEngine::Input::IsKeyPressed(tsEngine::KeyCode::S))
            move.Position.y -= speed;
    }

    virtual void OnDestroy() override
    {
    }
};