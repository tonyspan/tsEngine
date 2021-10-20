#include "pch.h"

#include "Engine/Log/Log.h"

#include "AudioManager.h"

#include <SDL_mixer.h>

#include "Music.h"

namespace tsEngine
{
	AudioManager::AudioManager()
	{
		LOG_INFO("Initializing AudioManager");

		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
			LOG_WARN("SDL_mixer initialization failed: {0}", Mix_GetError());
	}

	AudioManager::~AudioManager()
	{
		LOG_INFO("Shutting down AudioManager");

		Mix_CloseAudio();

		m_Sounds.clear();
	}

	void AudioManager::LoadMusic(const std::string& path)
	{
		m_MusicTrack = CreateRef<Music>(path);
	}

	void AudioManager::PlayMusic()
	{
		if (Mix_FadeInMusic(m_MusicTrack->Mus, -1, 2000) == -1)
		{
			LOG_INFO("{0}", Mix_GetError());
		}
	}

	void AudioManager::PauseMusic()
	{
		Mix_PauseMusic();
	}

	void AudioManager::ResumePlayingMusic()
	{
		Mix_ResumeMusic();
	}

	void AudioManager::ToggleMusicPlaying()
	{
		if (m_IsPlaying)
			PauseMusic();
		else
			ResumePlayingMusic();

		m_IsPlaying = !m_IsPlaying;
	}

	void AudioManager::LoadSound(const std::string& soundName, const std::string& path)
	{
		m_Sounds.emplace_back(soundName, path);
	}

	void AudioManager::PlaySound(const std::string& soundName)
	{
		auto found = std::find_if(m_Sounds.begin(), m_Sounds.end(), [&](auto& elem) {return elem.SoundName == soundName; });

		if (found == m_Sounds.end())
		{
			LOG_INFO("There is not such sound loaded: {0}", soundName);
			
			return;
		}

		int channelNo = Mix_PlayChannel(-1, found->SoundChunk, 0);
		
		if (channelNo == -1)
		{
			LOG_WARN("{0}", Mix_GetError());
		}

		Mix_Volume(-1, 128);
	}

	void AudioManager::StopSound(const std::string& soundName)
	{
		auto found = std::find_if(m_Sounds.begin(), m_Sounds.end(), [&](auto& elem) {return elem.SoundName == soundName; });

		if (found == m_Sounds.end())
		{
			LOG_INFO("There is not such sound loaded: {0}", soundName);
			
			return;
		}

		found->SoundChunk->volume = 0;
	}
}