#pragma once

#include <Core/Application.h>

class GameApp : public tsEngine::Application
{
public:
	GameApp();
	~GameApp();
};

tsEngine::Application* tsEngine::CreateApplication()
{
	return new GameApp();
}