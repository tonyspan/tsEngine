#include "pch.h"

#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

int main(int argc, char** argv)
{
    tsEngine::Log::Init();
    LOG_INFO("Initialized Log");

    auto app = tsEngine::CreateApplication();

    app->Run();

    delete app;

    return 0;
}
