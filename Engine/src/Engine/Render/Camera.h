#pragma once

// TODO: Reduce dependency between Components and Camera

namespace tsEngine
{
	struct TransformComponent;
	struct CameraComponent;

	struct CameraData
	{
		TransformComponent* Transform = nullptr;
		CameraComponent* Camera = nullptr;

		CameraData() = default;

		void Set(TransformComponent& tc, CameraComponent& cc);

		void Reset();

		operator bool() const { return (Transform != nullptr) || (Camera != nullptr); }
	};
}