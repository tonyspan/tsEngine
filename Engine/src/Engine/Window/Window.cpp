#include "pch.h"

#include "Window.h"

#include "SDLWindow.h"

namespace tsEngine
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<SDLWindow>(props);
	}
}