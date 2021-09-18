#include "GameApp.h"

GameApp::GameApp(const std::string& name)
	: tsEngine::Application(name)
{
}

GameApp::~GameApp()
{
}

tsEngine::Application* tsEngine::CreateApplication()
{
	return new GameApp("Game");
}
