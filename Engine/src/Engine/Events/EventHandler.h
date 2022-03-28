#pragma once

#include "Engine/Core/Base.h"

typedef union SDL_Event;

namespace tsEngine
{
	struct Event;
	class Window;

	class EventHandler
	{
		using EventFn = std::function<void(Event&)>;
	public:
		EventHandler(Window* window);

		void OnUpdate();
		void SetEventCallback(const EventFn& callback) { m_Callback = callback; }

		SDL_Event* NativeEvent() { return m_NativeHandler.get(); }
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
		Scope<SDL_Event> m_NativeHandler;
		EventFn m_Callback;
	};
}