#pragma once

#include "Engine.h"

namespace Game
{
	class PlayerController
	{
	public:
		void OnCreate(const tsEngine::Ref<tsEngine::EntityManager>& entityManager);
		void OnUpdate(float ts);
		void OnEvent(tsEngine::Event& event);

		void Reset();
	private:
		void OnKeyPressedEvent(tsEngine::KeyboardEvent& event);
		void OnMousePressedEvent(tsEngine::MouseButtonEvent& event);
		void OnMouseMoveEvent(tsEngine::MousePositionEvent& event);
	private:
		int m_Score;
		tsEngine::Ref<tsEngine::EntityManager> m_Context;
	};
}