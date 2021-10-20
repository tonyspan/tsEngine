#include "pch.h"

#include "AssetManager.h"

#include "Engine/Log/Log.h"

namespace tsEngine
{
    std::unordered_map<std::string, Scope<Texture>> AssetManager::s_Textures;
    std::unordered_map<std::string, int> AssetManager::s_Fonts;

    void AssetManager::CreateTexture(Renderer* renderer, const std::string& texName, const std::string& path)
    {
        auto found = std::find_if(s_Textures.begin(), s_Textures.end(), [&](const auto& pair) { return texName == pair.first; });

        if (found != s_Textures.end())
            LOG_WARN("Duplicate texture: {0} in path: {1}", texName, path);

        s_Textures.emplace(texName, CreateScope<Texture>(renderer, path));
    }

    void AssetManager::CreateTexture(Renderer* renderer, const std::string& path)
    {
        auto found = std::find_if(s_Textures.begin(), s_Textures.end(), [&](const auto& pair) { return path == pair.first; });

        if (found == s_Textures.end())
            s_Textures.emplace(path, CreateScope<Texture>(renderer, path));
    }

    Texture* AssetManager::GetTexture(const std::string& name)
    {
        auto found = std::find_if(s_Textures.begin(), s_Textures.end(), [&](const auto& pair) { return name == pair.first; });
        
        if (found != s_Textures.end())
        {
            return s_Textures[name].get();
        }
        else
        {
            LOG_ERROR("Could not load texture {0}", name);
            
            return nullptr;
        }
    }

    void AssetManager::AddFont(const std::string& path, int fontSize)
    {
        auto found = std::find_if(s_Fonts.begin(), s_Fonts.end(), [&](const auto& pair) { return path == pair.first; });

        if (found == s_Fonts.end())
            s_Fonts.emplace(path, fontSize);

        //renderer->OpenFont(path, fontSize);
    }

    void AssetManager::LoadFont(Renderer* renderer, const std::string& path)
    {
        auto found = std::find_if(s_Fonts.begin(), s_Fonts.end(), [&](const auto& pair) { return path == pair.first; });

        if (found != s_Fonts.end())
        {
            renderer->OpenFont(found->first, found->second);
        }
        else
        {
            std::filesystem::path filename = path;
            LOG_ERROR("Could not load font {0}", filename.filename().string());
        }
    }
}
