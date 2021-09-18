#pragma once

#include <Engine.h>

class GameApp : public tsEngine::Application
{
public:
	GameApp(const std::string& name);
	~GameApp();
};