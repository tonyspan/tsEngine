#pragma once

#include "pch.h"

#include "Events.h"
#include "MouseEvents.h"
#include "WindowEvents.h"
#include "KeyboardEvents.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keyCode);

		static std::string& UserInput();

		static bool MouseButtonDown();
		static bool MouseButtonClicked();
		static glm::vec2 MousePosition();
		static MouseButtonId MouseButtonId();

		static glm::vec2 MouseMotion();
		static int MouseWheelDirection();
		static void ResetWheel();
	};
}