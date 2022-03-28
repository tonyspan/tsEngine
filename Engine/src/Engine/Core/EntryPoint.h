//#include "pch.h"

#include "Engine/Core/Application.h"

extern tsEngine::Application* tsEngine::CreateApplication();

int main(int argc, char* args[])
{
    auto app = tsEngine::CreateApplication();

    bool success = app->Init();
    if (!success)
    {
        LOG_CRITICAL("Failed to initialize the application!");
        return 1;
    }

    int retval = app->Run();

    success = app->Shutdown();
    if (!success)
    {
        LOG_CRITICAL("Failed to uninitialize the application!");
        return 2;
    }

    delete app;

    return retval;
}
