#include "pch.h"

#include "Texture.h"

#include "Engine/Log/Log.h"

#include <SDL.h>
#include <SDL_image.h>

namespace tsEngine
{
    Texture::Texture(Renderer* renderer, const std::string& path)
    {
        LoadTexture(renderer, path);
    }

    bool Texture::LoadTexture(Renderer* renderer, const std::string& path)
    {
        Tex.reset(IMG_LoadTexture(renderer->GetNativeRenderer(), path.c_str()));

        if (Tex == nullptr)
        {
            LOG_ERROR("Unable to load: {0}, SDL_Image returned error {1}", path, IMG_GetError());
            
            return false;
        }

        return true;
    }

    SDL_Texture* Texture::GetRaw(Texture* tex)
    {
        return tex->Tex.get();
    }
}
