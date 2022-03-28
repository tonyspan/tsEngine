#pragma once

#include "Events.h"
#include "KeyCodes.h"

namespace tsEngine
{
	struct KeyboardEvent : public Event
	{
		KeyboardEvent(bool pressed, int repeat, KeyCode scancode)
			: Event("KeyboardEvent"), Pressed(pressed), Repeat(repeat), Scancode(scancode)
		{
		}
		
		virtual ~KeyboardEvent() = default;

		bool Pressed;
		int Repeat;
		KeyCode Scancode;
	};

	struct KeyboardTypedEvent : public Event
	{
		KeyboardTypedEvent(const std::string& input)
			: Event("KeyboardTypedEvent"), InputText(input)
		{
		}

		virtual ~KeyboardTypedEvent() = default;

		static inline void StartTextInput() { SDL_StartTextInput(); }
		static inline void StopTextInput() { SDL_StopTextInput(); }

		std::string InputText;
	};
}