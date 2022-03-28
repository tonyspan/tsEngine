#include "pch.h"

#include "Engine/Log/Log.h"

#include "AudioSystem.h"

#include <SDL_mixer.h>

#include "Music.h"

namespace tsEngine
{
	void AudioSystem::Init()
	{
		LOG_INFO("Initializing AudioSystem");

		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
			LOG_WARN("SDL_mixer initialization failed: {0}", Mix_GetError());
	}

	void AudioSystem::Shutdown()
	{
		LOG_INFO("Shutting down AudioSystem");

		Mix_CloseAudio();

		s_Sounds.clear();
	}

	void AudioSystem::LoadMusic(const std::string& path)
	{
		s_MusicTrack = CreateRef<Music>(path);
	}

	void AudioSystem::PlayMusic()
	{
		if (Mix_FadeInMusic(s_MusicTrack->m_Music, -1, 2000) == -1)
			LOG_INFO("{0}", Mix_GetError());
	}

	void AudioSystem::PauseMusic()
	{
		Mix_PauseMusic();
	}

	void AudioSystem::ResumePlayingMusic()
	{
		Mix_ResumeMusic();
	}

	void AudioSystem::ToggleMusicPlaying()
	{
		if (s_IsPlaying)
			PauseMusic();
		else
			ResumePlayingMusic();

		s_IsPlaying = !s_IsPlaying;
	}

	void AudioSystem::LoadSound(const std::string& soundName, const std::string& path)
	{
		s_Sounds.emplace_back(soundName, path);
	}

	void AudioSystem::PlaySound(const std::string& soundName)
	{
		auto found = std::find_if(s_Sounds.begin(), s_Sounds.end(), [&](auto& elem) {return elem.m_SoundName == soundName; });

		if (found == s_Sounds.end())
		{
			LOG_INFO("There is not such sound loaded: {0}", soundName);
			
			return;
		}

		if (Mix_PlayChannel(-1, found->m_SoundChunk, 0) == -1)
			LOG_WARN("{0}", Mix_GetError());

		Mix_Volume(-1, 128);
	}

	void AudioSystem::StopSound(const std::string& soundName)
	{
		auto found = std::find_if(s_Sounds.begin(), s_Sounds.end(), [&](auto& elem) {return elem.m_SoundName == soundName; });

		if (found == s_Sounds.end())
		{
			LOG_INFO("There is not such sound loaded: {0}", soundName);
			
			return;
		}

		found->m_SoundChunk->volume = 0;
	}
}