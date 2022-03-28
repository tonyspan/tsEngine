#pragma once

#include "Events.h"
#include "MouseCodes.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	struct MouseButtonEvent : public Event
	{
		MouseButtonEvent(bool down, int id, int clicks, bool clicked)
			: Event("MouseButtonEvent"), Down(down), ButtonId(static_cast<MouseCode>(id)), Clicks(clicks), Clicked(clicked)
		{
		}

		virtual ~MouseButtonEvent() = default;

		bool Down;
		MouseCode ButtonId;
		int Clicks;
		bool Clicked;
	};

	struct MousePositionEvent : public Event
	{
		MousePositionEvent(const glm::vec2& pos, const glm::vec2& motion)
			: Event("MousePositionEvent"), Position(pos), Motion(motion)
		{
		}

		virtual ~MousePositionEvent() = default;

		glm::vec2 Position;
		glm::vec2 Motion;
	};

	struct MouseWheelEvent : public Event
	{
		MouseWheelEvent(int direction)
			: Event("MouseWheelEvent"), Direction(direction)
		{
		}
		
		virtual ~MouseWheelEvent() = default;

		int Direction;
	};
}