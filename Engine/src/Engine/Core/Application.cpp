#include "pch.h"

#include "Application.h"

namespace tsEngine
{
    Application::Application(const std::string& name)
    {
        m_Window = CreateScope<Window>(WindowData{ name });

        if (!m_Window)
            LOG_CRITICAL("Unable to create a window.");
    }

    void Application::Run()
    {
        auto previousFrameTime = SDL_GetPerformanceCounter();

        SDL_Event event;
        while (m_Running)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT)
                    m_Running = false;
            }

            auto frameTime = SDL_GetPerformanceCounter();

            float deltaTime = (frameTime - previousFrameTime) / static_cast<float>(SDL_GetPerformanceFrequency());

            //LOG_INFO("Current FPS: {}", 1.f / deltaTime);

            previousFrameTime = frameTime;
        }
    }
}
