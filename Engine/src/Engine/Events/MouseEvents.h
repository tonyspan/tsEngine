#pragma once

#include "Events.h"

#include <glm/glm.hpp>

namespace tsEngine
{
	
	enum class MouseButtonId
	{
		LMB = 1, MMB = 2, RMB = 3
	};

	struct MouseButtonEvent : public Event
	{
		MouseButtonEvent(bool down, int id, int clicks, bool clicked)
			: Event("Mouse"), Down(down), ButtonId(static_cast<MouseButtonId>(id)), Clicks(clicks), Clicked(clicked)
		{
		}
		
		virtual ~MouseButtonEvent() = default;

		std::string GetName() const override { return Name; }

		bool Down;
		MouseButtonId ButtonId;
		int Clicks;
		bool Clicked;
	};

	struct MousePositionEvent : public Event
	{
		MousePositionEvent(const glm::vec2& pos, const glm::vec2& motion)
			: Event("Position"), Position(pos), Motion(motion)
		{
		}

		virtual ~MousePositionEvent() = default;

		std::string GetName() const override { return Name; }

		glm::vec2 Position;
		glm::vec2 Motion;
	};

	struct MouseWheelEvent : public Event
	{
		MouseWheelEvent(int direction)
			: Event("Wheel"), Direction(direction)
		{
		}
		
		virtual ~MouseWheelEvent() = default;

		std::string GetName() const override { return Name; }

		int Direction;
	};
}