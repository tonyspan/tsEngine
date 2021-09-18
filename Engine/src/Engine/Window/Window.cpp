#include "pch.h"

#include "Window.h"
#include "Engine/Log/Log.h"

namespace tsEngine
{
    bool Window::Init(const WindowData& props)
    {
        m_WindowData.Title = props.Title;
        m_WindowData.Width = props.Width;
        m_WindowData.Height = props.Height;
        m_WindowData.VSync = props.VSync;

        LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        m_NativeWindow.reset(SDL_CreateWindow(m_WindowData.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_WindowData.Width, m_WindowData.Height, 0));

        if (!m_NativeWindow)
        {
            LOG_CRITICAL("Unable to create a window. SDL Error: {0}", SDL_GetError());
            
            return false;
        }

        //SDL_HideWindow(m_NativeWindowHandle.get());
        
        return true;
    }

    bool Window::Shutdown()
    {
        LOG_INFO("Destroying window");

        return true;
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
