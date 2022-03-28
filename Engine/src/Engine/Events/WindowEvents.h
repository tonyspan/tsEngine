#pragma once

#include "Events.h"

namespace tsEngine
{
	struct QuitEvent : public Event
	{
		QuitEvent(bool closed)
			: Event("QuitEvent"), Closed(closed)
		{
		}
		
		virtual ~QuitEvent() = default;

		bool Closed;
	};

	struct ResizeEvent : public Event
	{
		ResizeEvent(int width, int height)
			: Event("ResizeEvent"), NewWidth(width), NewHeight(height)
		{
		}
		
		virtual ~ResizeEvent() = default;

		int NewWidth, NewHeight;
	};

}