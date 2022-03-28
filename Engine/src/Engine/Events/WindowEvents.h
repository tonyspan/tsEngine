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
		ResizeEvent(uint32_t width, uint32_t height)
			: Event("ResizeEvent"), NewWidth(width), NewHeight(height)
		{
		}
		
		virtual ~ResizeEvent() = default;

		uint32_t NewWidth, NewHeight;
	};

}