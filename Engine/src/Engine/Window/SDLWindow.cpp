#include "pch.h"

#include "SDLWindow.h"

#include "Engine/Log/Log.h"

#include <SDL.h>
#include <SDL_image.h>

namespace tsEngine
{
	void Deleter2<SDL_Window>::operator()(SDL_Window* ptr)
	{
		SDL_DestroyWindow(ptr);
	}

	SDLWindow::SDLWindow(const WindowProps& props)
	{
		Init(props);
		AddWindowIcon();
	}

	SDLWindow::~SDLWindow()
	{
		LOG_INFO("Destroying window");
	}

	void SDLWindow::AddWindowIcon()
	{
		SDL_Surface* iconSurface = IMG_Load(m_Data.Icon.c_str());

		if (iconSurface)
		{
			SDL_SetWindowIcon(m_NativeWindow.get(), iconSurface);
			SDL_FreeSurface(iconSurface);
		}
	}

	void SDLWindow::SetWidth(uint32_t width)
	{
		m_Data.Width = width;
	}

	void SDLWindow::SetHeight(uint32_t height)
	{
		m_Data.Height = height;
	}

	uint32_t SDLWindow::GetWidth() const
	{
		return m_Data.Width;
	}

	uint32_t SDLWindow::GetHeight() const
	{
		return m_Data.Height;
	}

	void SDLWindow::SetVSync(bool enabled)
	{
		m_Data.VSync = enabled;
	}

	bool SDLWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void* SDLWindow::GetNativeWindow() const
	{
		return m_NativeWindow.get();
	}

	void SDLWindow::Init(const WindowProps& props)
	{
		m_Data = props;

		LOG_INFO("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

		auto flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

		if (m_Data.Maximized)
			flags = (SDL_WindowFlags)(flags | SDL_WINDOW_MAXIMIZED);

		m_NativeWindow.reset(SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Data.Width, m_Data.Height, flags));

		ASSERT(m_NativeWindow, "Unable to create a window. SDL Error: {}", SDL_GetError());
	}
}