#include "pch.h"

#include "Engine/Log/Log.h"

#include "Music.h"

#include <SDL_mixer.h>

namespace tsEngine
{
    Music::Music(const std::string& path)
    {
        LoadMusic(path);
    }

    Music::~Music()
    {
        if (Mus != nullptr)
        {
            Mix_FreeMusic(Mus);
        }
    }

    bool Music::LoadMusic(const std::string& path)
    {
        if (Mus != nullptr)
        {
            LOG_WARN("Overwriting already loaded music with: {0}", path);
            Mix_FreeMusic(Mus);
        }

        Mus = Mix_LoadMUS(path.c_str());

        if (Mus == nullptr)
        {
            LOG_ERROR("Unable to load: {0}, Mix_LoadMUS error: {1}", path, Mix_GetError());
            return false;
        }

        return true;
    }
}