#include "pch.h"

#include "Camera.h"

#include "Engine/ECS/Components.h"

namespace tsEngine
{
	void CameraData::Set(TransformComponent& tc, CameraComponent& cc)
	{
		Transform = &tc;
		Camera = &cc;
	}

	void CameraData::Reset()
	{
		Transform = nullptr;
		Camera = nullptr;
	}
}