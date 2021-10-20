#pragma once

#include "pch.h"
#include "Engine/Core/Base.h"

#include <SDL.h>

namespace tsEngine
{
	constexpr uint32_t DEFAULT_WIDTH = 1600;
	constexpr uint32_t DEFAULT_HEIGHT = 900;
	
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		std::string Icon;
		bool VSync = false;

		WindowProps(const std::string& title = "tsEngine", uint32_t width = DEFAULT_WIDTH, uint32_t height = DEFAULT_HEIGHT)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		Window(const WindowProps& props = WindowProps());
		~Window();

		SDL_Window* GetNativeWindow() const;

		void SetWidth(uint32_t width) { m_WindowData.Width = width; }
		void SetHeight(uint32_t height) { m_WindowData.Height = height; }

		uint32_t GetWidth() { return m_WindowData.Width; }
		uint32_t GetHeight() { return m_WindowData.Height; }

		bool GetVSync() { return m_WindowData.VSync; }

		void SetWindowData(const WindowProps& props) { m_WindowData = props; }

	private:
		Window(const Window& other) = delete;
		Window& operator=(Window& other) = delete;

		bool Init(const WindowProps& props);
		bool Shutdown();
		void AddWindowIcon();
	private:
		WindowProps m_WindowData;
		Scope2<SDL_Window, SDL_DestroyWindow> m_NativeWindow;
	};
}

