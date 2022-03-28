#pragma once

#include "Engine/Core/Base.h"

#include "Sound.h"

namespace tsEngine
{
	struct Music;

	class AudioSystem
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadMusic(const std::string& path);
		static void PlayMusic();
		static void PauseMusic();
		static void ResumePlayingMusic();

		static void ToggleMusicPlaying();

		static void LoadSound(const std::string& soundName, const std::string& path);
		static void PlaySound(const std::string& soundName);
		static void StopSound(const std::string& soundName);
	private:
		static inline Ref<Music> s_MusicTrack;
		static inline bool s_IsPlaying = true;
		
		static inline std::vector<Sound> s_Sounds;
	};
}
