#include "pch.h"

#include "Core/Application.h"

int main(int argc, char** argv)
{
    auto app = tsEngine::CreateApplication();

    app->Run();

    delete app;
}
