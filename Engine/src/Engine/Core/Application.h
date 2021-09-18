#pragma once

#include "pch.h"

#include "Engine/Core/Base.h"
#include "Engine/Log/Log.h"

#include "Engine/Window/Window.h"

namespace tsEngine
{
	class Application
	{
	public:
		Application(const std::string& name);
		virtual ~Application() = default;

		void Run();
	private:
		Scope<Window> m_Window;
		bool m_Running;
	};

	// To be defined in the client
	extern Application* CreateApplication();
}

