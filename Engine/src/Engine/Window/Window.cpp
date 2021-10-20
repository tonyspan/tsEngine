#include "pch.h"

#include "Window.h"
#include "Engine/Log/Log.h"

#include <SDL_image.h>

namespace tsEngine
{
    void Window::AddWindowIcon()
    {
        SDL_Surface* iconSurface = IMG_Load(m_WindowData.Icon.c_str());

        if (iconSurface)
        {
            SDL_SetWindowIcon(m_NativeWindow.get(), iconSurface);
            SDL_FreeSurface(iconSurface);
        }
    }

    bool Window::Init(const WindowProps& props)
    {
        m_WindowData = props;

        LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        auto flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        m_NativeWindow.reset(SDL_CreateWindow(m_WindowData.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_WindowData.Width, m_WindowData.Height, flags));

        AddWindowIcon();

        if (!m_NativeWindow)
        {
            LOG_CRITICAL("Unable to create a window. SDL Error: {0}", SDL_GetError());
            
            return false;
        }
        
        return true;
    }

    bool Window::Shutdown()
    {
        LOG_INFO("Destroying window");

        return true;
    }

    Window::Window(const WindowProps& props)
    {
        Init(props);
    }

    SDL_Window* Window::GetNativeWindow() const
    {
        return m_NativeWindow.get();
    }

    Window::~Window()
    {
        Shutdown();
    }
}
