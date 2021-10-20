#pragma once

#include "Events.h"
#include "KeyCodes.h"

namespace tsEngine
{
	struct KeyboardEvent : public Event
	{
		KeyboardEvent(bool pressed, int repeat, KeyCode scancode)
			: Event("Keyboard"), Pressed(pressed), Repeat(repeat), Scancode(scancode)
		{
		}
		
		virtual ~KeyboardEvent() = default;

		std::string GetName() const override { return Name; }

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
		
		std::string GetName() const  override{ return Name; }

		static inline void StartTextInput() { SDL_StartTextInput(); }
		static inline void StopTextInput() { SDL_StopTextInput(); }

		std::string InputText;
	};
}