#pragma once

#include "Engine/Core/Base.h"

#include "Music.h"
#include "Sound.h"

namespace tsEngine
{
	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		void LoadMusic(const std::string& path);
		void PlayMusic();
		void PauseMusic();
		void ResumePlayingMusic();

		void ToggleMusicPlaying();

		void LoadSound(const std::string& soundName, const std::string& path);
		void PlaySound(const std::string& soundName);
		void StopSound(const std::string& soundName);
	private:
		AudioManager(const AudioManager& other) = delete;
		AudioManager& operator=(AudioManager& other) = delete;
	private:
		Ref<Music> m_MusicTrack;
		bool m_IsPlaying = true;
		
		std::vector<Sound> m_Sounds;
	};
}
