#pragma once

#include "Events.h"
#include "MouseEvents.h"
#include "WindowEvents.h"
#include "KeyboardEvents.h"

#include "Engine/Window/Window.h"

namespace tsEngine
{
	using EventFn = std::function<void(Event&)>;

	class EventHandler
	{
	public:
		EventHandler(Window* window);

		void OnUpdate();
		void SetEventCallback(const EventFn& callback) { m_Callback = callback; }

		SDL_Event* NativeEvent() { return &m_NativeHandler; }
	private:
		void OnQuit();
		void OnKeyPressed();
		void OnKeyTypedEvent();
		void OnWindowResize();
		void OnMousePressed();
		void OnMousePositon();
		void OnMouseWheel();
	private:
		Window* m_Window;
		SDL_Event m_NativeHandler;
		EventFn m_Callback;
	};
}