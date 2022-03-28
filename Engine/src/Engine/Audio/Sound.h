#pragma once

#include "pch.h"

struct Mix_Chunk;

namespace tsEngine
{
	struct Sound
	{
	public:
		Sound(const std::string& soundName, const std::string& path);
		~Sound();
		
		bool LoadSound(const std::string& soundName, const std::string& path);
	private:
		Mix_Chunk* m_SoundChunk{};
		std::string m_SoundName;

		friend class AudioSystem;
	};
}