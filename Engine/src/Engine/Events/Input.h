#pragma once

#include "pch.h"

//#include "Events.h"
//#include "MouseEvents.h"
//#include "WindowEvents.h"
//#include "KeyboardEvents.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	struct Events;
	enum class MouseCode;
	enum class KeyCode;

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keyCode);

		static std::string& UserInput();

		static bool Input::IsMousePressed(MouseCode mouseButton);
		static bool MouseButtonDown();
		static bool MouseButtonClicked();
		static glm::vec2& MousePosition();

		static glm::vec2& MouseMotion();
		static int MouseWheelDirection();
		static void ResetWheel();
	};
}