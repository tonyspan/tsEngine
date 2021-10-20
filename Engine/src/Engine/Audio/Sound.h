#pragma once

#include "pch.h"

struct Mix_Chunk;

namespace tsEngine
{
	struct Sound
	{
		Mix_Chunk* SoundChunk{};
		std::string SoundName;

		Sound(const std::string& soundName, const std::string& path);
		~Sound();
		
		bool LoadSound(const std::string& soundName, const std::string& path);
	};
}