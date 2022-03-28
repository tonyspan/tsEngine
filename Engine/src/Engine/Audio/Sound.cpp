#include "pch.h"

#include "Engine/Log/Log.h"

#include "Sound.h"

#include <SDL_mixer.h>

namespace tsEngine
{
    Sound::Sound(const std::string& soundName, const std::string& path)
    {
        LoadSound(soundName, path);
    }

    Sound::~Sound()
    {
        if (m_SoundChunk != nullptr)
            Mix_FreeChunk(m_SoundChunk);
    }

    bool Sound::LoadSound(const std::string& soundName, const std::string& path)
    {
        if (m_SoundChunk != nullptr)
        {
            LOG_WARN("Overwriting already loaded sound with: {0}", path);
            Mix_FreeChunk(m_SoundChunk);
        }

        m_SoundChunk = Mix_LoadWAV(path.c_str());

        if (m_SoundChunk == nullptr)
        {
            LOG_ERROR("Unable to load: {0}, Mix_LoadMUS error: {1}", path, Mix_GetError());
            return false;
        }

        m_SoundName = soundName;

        return true;
    }
}
