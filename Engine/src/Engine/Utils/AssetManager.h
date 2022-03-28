#pragma once

#include "pch.h"

#include "Engine/Core/Base.h"

namespace tsEngine
{
	struct Texture;
	struct Font;

	class AssetManager
	{
	public:
		// NOTE: Add texture
		static void AddTexture(const std::string& texName, Ref<Texture>& texture);
		// Returns texture based either on texture name (Sandbox project) or path (Editor)
		static Ref<Texture> GetTexture(const std::string& name);

		static void AddFont(const std::string& fontId, Ref<Font>& font);
		static Ref<Font> GetFont(const std::string& fontId);

		// Clear AssetManager or TTF_Quit() crashes on shutdown :)
		static void Clear();
	public:
		// NOTE: Probably `std::filesystem::current_path()` won't always work as it is now
		static inline const std::filesystem::path s_BasePath = (std::filesystem::current_path() / "src" += std::filesystem::path("\\").make_preferred()).make_preferred();
		static inline const std::filesystem::path s_AssetPath = (s_BasePath / "Assets" += std::filesystem::path("\\").make_preferred()).make_preferred();
		static inline const std::filesystem::path s_SciptPath = (s_BasePath / "Scripts" += std::filesystem::path("\\").make_preferred()).make_preferred();

		static inline const std::filesystem::path s_DefaultTexturePath = (s_AssetPath / "default.png").make_preferred();
		static inline const std::filesystem::path s_DefaultFontPath = (s_AssetPath / "default.ttf").make_preferred();

	private:
		static std::unordered_map<std::string, Ref<Texture>> s_Textures;
		static std::unordered_map<std::string, Ref<Font>> s_Fonts;
	};
}
