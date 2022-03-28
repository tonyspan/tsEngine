#pragma once

#include "Engine.h"

namespace Game
{
	class ObstacleController
	{
	public:
		void OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager);
		void OnUpdate(float ts);
		
		void Reset();
	private:
		float m_ObstacleWidth = 140.0f;
		float m_ObstacleHeight = 550.0f;
		int m_NumberOfObstacles = 4;

		tsEngine::Ref<tsEngine::EntityManager> m_Context;
	};
}