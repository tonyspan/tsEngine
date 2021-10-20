#include "pch.h"

#include "EventHandler.h"

//#include "Engine/Log/Log.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
//#include "imgui_impl_sdlrenderer.h"

namespace tsEngine
{	
	extern MouseButtonEvent s_MouseEvent;
	extern KeyboardEvent s_KeyboardEvent;
	extern MousePositionEvent s_MousePositionEvent;
	extern KeyboardTypedEvent s_KeyboardTypedEvent;
	extern MouseWheelEvent s_Wheel;

	EventHandler::EventHandler(Window* window)
		: m_Window(window)
	{
	}

	void EventHandler::OnUpdate()
	{
		while (SDL_PollEvent(&m_NativeHandler))
		{
			ImGui_ImplSDL2_ProcessEvent(&m_NativeHandler);

			// NOTE: Not sure if this is placed correctly here
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse || io.WantCaptureKeyboard)
				return;

			OnKeyPressed();
			OnKeyTypedEvent();
			OnQuit();
			OnMousePressed();
			OnMousePositon();
			OnWindowResize();
			OnMouseWheel();
		}
	}

	void EventHandler::OnQuit()
	{
		if (m_NativeHandler.type == SDL_QUIT)
		{
			QuitEvent quit(true);
			m_Callback(quit);
		}
	}

	void EventHandler::OnKeyPressed()
	{
		if (m_NativeHandler.type == SDL_KEYDOWN)
		{
			KeyboardEvent keyboard(true, m_NativeHandler.key.repeat, static_cast<KeyCode>(m_NativeHandler.key.keysym.scancode));
			s_KeyboardEvent = keyboard;
			m_Callback(keyboard);
		}
		else if (m_NativeHandler.type == SDL_KEYUP)
		{
			KeyboardEvent keyboard(false, 0, KeyCode::None);
			s_KeyboardEvent = keyboard;
			m_Callback(keyboard);
		}
	}

	void EventHandler::OnKeyTypedEvent()
	{
		if (m_NativeHandler.type == SDL_TEXTINPUT)
		{
			KeyboardTypedEvent input(m_NativeHandler.text.text);
			s_KeyboardTypedEvent = input;
			m_Callback(input);
		}
	}

	void EventHandler::OnMousePressed()
	{
		if (m_NativeHandler.type == SDL_MOUSEBUTTONDOWN)
		{
			MouseButtonEvent mouse(true, m_NativeHandler.button.button, m_NativeHandler.button.clicks, true);
			s_MouseEvent = mouse;
			m_Callback(mouse);
		}
		else if (m_NativeHandler.type == SDL_MOUSEBUTTONUP)
		{
			MouseButtonEvent mouse(false, m_NativeHandler.button.button, m_NativeHandler.button.clicks, false);
			s_MouseEvent = mouse;
			m_Callback(mouse);
		}
		else
		{
			s_MouseEvent.Clicked = false;
		}
	}

	void EventHandler::OnWindowResize()
	{
		if (m_NativeHandler.type == SDL_WINDOWEVENT)
		{
			if (m_NativeHandler.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				ResizeEvent resize(m_NativeHandler.window.data1, m_NativeHandler.window.data2);
				m_Callback(resize);
			}
		}
	}

	void EventHandler::OnMousePositon()
	{
		if (m_NativeHandler.type == SDL_MOUSEMOTION)
		{
			MousePositionEvent pos({ m_NativeHandler.button.x, m_NativeHandler.button.y }, { m_NativeHandler.motion.xrel, m_NativeHandler.motion.yrel });
			s_MousePositionEvent = pos;
			m_Callback(pos);
		}
	}

	void EventHandler::OnMouseWheel()
	{
		if (m_NativeHandler.type == SDL_MOUSEWHEEL)
		{
			MouseWheelEvent wheel(m_NativeHandler.wheel.y);
			s_Wheel = wheel;
			m_Callback(wheel);
		}
	}
}