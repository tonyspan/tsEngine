#pragma once

#include "pch.h"

namespace tsEngine
{
	class Application
	{
	public:
		virtual ~Application() = default;

		void Run();
	};

	// To be defined in the game
	extern Application* CreateApplication();
}

