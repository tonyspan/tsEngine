#include "pch.h"

#include "EventHandler.h"

//#include "Engine/Log/Log.h"

#include "Engine/Window/Window.h"

#include "Events.h"
#include "MouseEvents.h"
#include "WindowEvents.h"
#include "KeyboardEvents.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
//#include "imgui_impl_sdlrenderer.h"

#include <SDL.h>

namespace tsEngine
{	
	extern MouseButtonEvent g_MouseEvent;
	extern KeyboardEvent g_KeyboardEvent;
	extern MousePositionEvent g_MousePositionEvent;
	extern KeyboardTypedEvent g_KeyboardTypedEvent;
	extern MouseWheelEvent g_Wheel;

	EventHandler::EventHandler(Window* window)
		: m_Window(window), m_NativeHandler(CreateScope<SDL_Event>())
	{
	}

	void EventHandler::OnUpdate()
	{
		while (SDL_PollEvent(m_NativeHandler.get()))
		{
			ImGui_ImplSDL2_ProcessEvent(m_NativeHandler.get());

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
		if (m_NativeHandler->type == SDL_QUIT)
		{
			QuitEvent quit(true);
			m_Callback(quit);
		}
	}

	void EventHandler::OnKeyPressed()
	{
		if (m_NativeHandler->type == SDL_KEYDOWN)
		{
			KeyboardEvent keyboard(true, m_NativeHandler->key.repeat, static_cast<KeyCode>(m_NativeHandler->key.keysym.scancode));
			g_KeyboardEvent = keyboard;
			m_Callback(keyboard);
		}
		else if (m_NativeHandler->type == SDL_KEYUP)
		{
			KeyboardEvent keyboard(false, 0, KeyCode::None);
			g_KeyboardEvent = keyboard;
			m_Callback(keyboard);
		}
	}

	void EventHandler::OnKeyTypedEvent()
	{
		if (m_NativeHandler->type == SDL_TEXTINPUT)
		{
			KeyboardTypedEvent input(m_NativeHandler->text.text);
			g_KeyboardTypedEvent = input;
			m_Callback(input);
		}
	}

	void EventHandler::OnMousePressed()
	{
		if (m_NativeHandler->type == SDL_MOUSEBUTTONDOWN)
		{
			MouseButtonEvent mouse(true, m_NativeHandler->button.button, m_NativeHandler->button.clicks, true);
			g_MouseEvent = mouse;
			m_Callback(mouse);
		}
		else if (m_NativeHandler->type == SDL_MOUSEBUTTONUP)
		{
			MouseButtonEvent mouse(false, m_NativeHandler->button.button, m_NativeHandler->button.clicks, false);
			g_MouseEvent = mouse;
			m_Callback(mouse);
		}
		else
		{
			g_MouseEvent.Clicked = false;
		}
	}

	void EventHandler::OnWindowResize()
	{
		if (m_NativeHandler->type == SDL_WINDOWEVENT)
		{
			if (m_NativeHandler->window.event == SDL_WINDOWEVENT_RESIZED)
			{
				ResizeEvent resize(m_NativeHandler->window.data1, m_NativeHandler->window.data2);
				m_Callback(resize);
			}
		}
	}

	void EventHandler::OnMousePositon()
	{
		if (m_NativeHandler->type == SDL_MOUSEMOTION)
		{
			MousePositionEvent pos({ m_NativeHandler->button.x, m_NativeHandler->button.y }, { m_NativeHandler->motion.xrel, m_NativeHandler->motion.yrel });
			g_MousePositionEvent = pos;
			m_Callback(pos);
		}
	}

	void EventHandler::OnMouseWheel()
	{
		if (m_NativeHandler->type == SDL_MOUSEWHEEL)
		{
			MouseWheelEvent wheel(m_NativeHandler->wheel.y);
			g_Wheel = wheel;
			m_Callback(wheel);
		}
	}
}