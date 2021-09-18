#pragma once

#include "pch.h"
#include "Engine/Core/Base.h"

#include <SDL.h>

constexpr uint32_t DEFAULT_WIDTH = 1600;
constexpr uint32_t DEFAULT_HEIGHT = 900;

namespace tsEngine
{
	struct WindowData
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool VSync = false;

		WindowData(const std::string& title = "tsEngine", uint32_t width = DEFAULT_WIDTH, uint32_t height = DEFAULT_HEIGHT)
			: Title(title), Width(width), Height(height)
		{
		}
	};


    class Window
    {
    public:
        Window(const WindowData& props = WindowData()) { Init(props); }
        ~Window();

        SDL_Window* GetNativeWindow() const;

    private:
        Window(const Window& other) = delete;
        Window& operator=(Window& other) = delete;

        bool Init(const WindowData& props);
        bool Shutdown();

        struct DestroySDLWwindow
        {
            void operator()(SDL_Window* ptr)
            {
                SDL_DestroyWindow(ptr);
            }

        };
    private:
        WindowData m_WindowData;
        std::unique_ptr<SDL_Window, DestroySDLWwindow> m_NativeWindow;
    };
}

