#include "GameApplication.h"

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

tsEngine::Application* tsEngine::CreateApplication()
{
	return new GameApp();
}
