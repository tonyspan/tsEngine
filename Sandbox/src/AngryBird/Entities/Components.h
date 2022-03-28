#pragma once

namespace Game
{
	struct PlayerComponent
	{
		float PanSpeed;
		bool StillPlaying = false;

		PlayerComponent(float speed)
			: PanSpeed(speed) {}
		PlayerComponent(float speed, bool stillPlaying)
			: PanSpeed(speed), StillPlaying(stillPlaying) {}
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

	struct ScoreComponent
	{
		int Score = 0;
	};

	struct ScoreColliderComponent
	{
		float Speed = 150.0f;
		bool Triggered = false;
	};

	struct HealthComponent
	{
		int Lives = 0;

		HealthComponent() = default;
		HealthComponent(int lives)
			: Lives(lives) {}

		operator bool() { return Lives > 0; }
	};
}