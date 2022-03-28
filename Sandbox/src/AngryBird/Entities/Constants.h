#pragma once

#include "../../pch.h"

namespace Game::Constants
{
	constexpr uint32_t WIDTH = 1280;
	constexpr uint32_t HEIGHT = 720;
	constexpr char* TITLE = "AngryBird";
	constexpr char* ICONPATH = "src/AngryBird/Assets/angry.png";

	constexpr char* PLAYER = "src/AngryBird/Assets/angry.png";
	constexpr char* BG = "src/AngryBird/Assets/background2.jpg";
	constexpr char* PIPE = "src/AngryBird/Assets/pipe.png";
	constexpr char* FONT = "src/AngryBird/Assets/font.ttf";
	
	constexpr char* DEATH = "src/AngryBird/Assets/mb_die.wav";
	constexpr char* MUSIC = "src/AngryBird/Assets/angry.mp3";

	constexpr char* MAIN = "src/AngryBird/Assets/main.jpg";

	enum class GameState
	{
		MainMenu = 0,
		EndMenu,
		Play
	};
}