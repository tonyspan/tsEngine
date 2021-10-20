#pragma once

namespace Game
{
    struct PlayerComponent
    {
        float PanSpeed;
        PlayerComponent(float speed)
            : PanSpeed(speed) {}
    };

    struct ObstacleComponent
    {
        float Speed;
        ObstacleComponent(float speed)
            : Speed(speed) {}
    };

    struct WallComponent
    {
    };

    struct ScoreColliderComponent
    {
        float Speed = 150.0f;
        bool Triggered = false;
    };
}