#pragma once

#include "Engine/Render/Renderer.h"

#include <SDL_ttf.h>

struct SDL_Texture;

namespace tsEngine
{
    struct Texture
    {
        Scope2<SDL_Texture, SDL_DestroyTexture> Tex;

        Texture() = default;
        Texture(Renderer* renderer, const std::string& path);
        ~Texture() = default;
        
        bool LoadTexture(Renderer* renderer, const std::string& path);

        // Helper function
        // For usage on renderer, not to write sprite.Image->Tex.get()
        static SDL_Texture* GetRaw(Texture* tex);
    };
}
