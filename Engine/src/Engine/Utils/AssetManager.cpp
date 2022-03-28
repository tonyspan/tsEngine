#include "pch.h"

#include "AssetManager.h"

#include "Engine/Log/Log.h"

#include "Engine/Render/Texture.h"
#include "Engine/Render/Font.h"

namespace tsEngine
{
	std::unordered_map<std::string, Ref<Texture>> AssetManager::s_Textures;
	std::unordered_map<std::string, Ref<Font>> AssetManager::s_Fonts;

	void AssetManager::AddTexture(const std::string& texName, Ref<Texture>& texture)
	{
		auto found = std::find_if(s_Textures.begin(), s_Textures.end(), [&](const auto& pair) { return texName == pair.first; });
		 
		if (found != s_Textures.end())
		{
			LOG_WARN("Duplicate texture: {} in path: {}", texName, texture->Path);
			return;
		}

		s_Textures.emplace(texName, std::move(texture));
	}

	Ref<Texture> AssetManager::GetTexture(const std::string& name)
	{
		auto found = std::find_if(s_Textures.begin(), s_Textures.end(), [&](const auto& pair) { return name == pair.first; });
		 
		if (found != s_Textures.end())
		{
			return s_Textures[name];
		}
		else
		{
			LOG_ERROR("Could not load texture {}", name);
			
			return nullptr;
		}
	}

	void AssetManager::AddFont(const std::string& fontId, Ref<Font>& font)
	{
		auto found = std::find_if(s_Fonts.begin(), s_Fonts.end(), [&](const auto& pair) { return fontId == pair.first; });

		if (found != s_Fonts.end())
		{
			LOG_WARN("Duplicate font: {} in path: {}", fontId, font->Path);
			
			return;
		}

		s_Fonts.emplace(fontId, std::move(font));
	}

	Ref<Font> AssetManager::GetFont(const std::string& fontId)
	{
		auto found = std::find_if(s_Fonts.begin(), s_Fonts.end(), [&](const auto& pair) { return fontId == pair.first; });

		if (found != s_Fonts.end())
		{
			return s_Fonts[fontId];
		}
		else
		{
			LOG_ERROR("Could not load font {}", fontId);

			return nullptr;
		}
	}

	void AssetManager::Clear()
	{
		s_Textures.clear();
		s_Fonts.clear();
	}
}
