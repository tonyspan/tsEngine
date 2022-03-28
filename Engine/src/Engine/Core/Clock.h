#pragma once

#include "pch.h"

// Similar functionality to SDL_GetPerformanceCounter etc.
class Clock
{
	using TimePoint = std::chrono::steady_clock::time_point;
	using floatDuration = std::chrono::duration<float>;
public:
	static TimePoint Now()
	{
		return std::chrono::high_resolution_clock::now();
	}

	static float Duration(TimePoint current, TimePoint previous)
	{
		return std::chrono::duration_cast<floatDuration>(current - previous).count();
	}

	static constexpr auto GetPerformanceCounter = &Clock::Now;
	static constexpr auto GetPerformanceDifference = &Clock::Duration;
};
