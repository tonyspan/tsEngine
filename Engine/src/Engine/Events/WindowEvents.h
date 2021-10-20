#pragma once

#include "Events.h"

namespace tsEngine
{
	struct QuitEvent : public Event
	{
		QuitEvent(bool closed)
			: Event("Quit"), Closed(closed)
		{
		}
		
		virtual ~QuitEvent() = default;

		std::string GetName() const override { return Name; }

		bool Closed;
	};

	struct ResizeEvent : public Event
	{
		ResizeEvent(int width, int height)
			: Event("Resize"), NewWidth(width), NewHeight(height)
		{
		}
		
		virtual ~ResizeEvent() = default;

		std::string GetName() const override { return Name; }

		int NewWidth, NewHeight;
	};

}