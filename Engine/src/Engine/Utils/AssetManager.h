#pragma once

#include "pch.h"

#include "Engine/Core/Base.h"

#include "Engine/Render/Texture.h"
#include "Engine/Render/Renderer.h"

namespace tsEngine
{
    class AssetManager
    {
    public:
        // NOTE: For use in Sandbox project
        static void CreateTexture(Renderer* renderer, const std::string& texName, const std::string& path);
        // NOTE: For use in Editor
        static void CreateTexture(Renderer* renderer, const std::string& path);
        // Returns texture based either on texture name (Sandbox project) or path (Editor)
        static Texture* GetTexture(const std::string& name);
        
        static void AddFont(const std::string& path, int fontSize);
        static void LoadFont(Renderer* renderer, const std::string& path);
    public:
        // NOTE: Probably `std::filesystem::current_path()` won't always work as it is now
        static inline const std::filesystem::path s_BasePath = std::filesystem::current_path() += "\\src\\";
        static inline const std::filesystem::path s_AssetPath = s_BasePath.string() + "Assets\\";
        static inline const std::filesystem::path s_SciptPath = s_BasePath.string() + "Scripts\\";
        
        static inline const std::filesystem::path s_DefaultTexturePath = s_AssetPath.string() + "default.png";
        static inline const std::filesystem::path s_DefaultFontPath = s_AssetPath.string() + "default.ttf";

    private:
        static std::unordered_map<std::string, Scope<Texture>> s_Textures;
        static std::unordered_map<std::string, int> s_Fonts;
    };
}
