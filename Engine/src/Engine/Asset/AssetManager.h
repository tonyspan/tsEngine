#pragma once

#include "pch.h"

#include "Engine/Core/Base.h"

#include "Engine/Log/Log.h"

namespace tsEngine
{
	class Texture;
	class Font;

	class Asset;

	// TODO: Improve interaction between Asset and AssetManager

	class AssetManager
	{
	public:
		template<typename T>
		static void AddAsset(const std::string& name, Ref<T>& asset)
		{
			ASSERT((std::is_base_of_v<Asset, T>), "Not an Asset");

			auto found = std::find_if(s_Assets.begin(), s_Assets.end(), [&](const auto& pair) { return name == pair.first; });

			if (found != s_Assets.end())
			{
				LOG_WARN("Duplicate asset: {} in path: {}", name, std::any_cast<Ref<T>>(s_Assets[name])->GetPath());
				return;
			}

			s_Assets.emplace(name, std::move(asset));
		}

		template<typename T>
		static Ref<T> GetAsset(const std::string& name)
		{
			auto found = std::find_if(s_Assets.begin(), s_Assets.end(), [&](const auto& pair) { return name == pair.first; });

			if (found != s_Assets.end())
			{
				return std::any_cast<Ref<T>>(s_Assets[name]);
			}
			else
			{
				LOG_ERROR("Could not load asset {}", name);

				return nullptr;
			}
		}

		static void Init();
		static void Clear();
	public:
		// TODO: Move the following elsewhere
		// NOTE: Probably `std::filesystem::current_path()` won't always work as it is now
		static inline const std::filesystem::path s_BasePath = (std::filesystem::current_path() / "src" += std::filesystem::path("\\").make_preferred()).make_preferred();
		static inline const std::filesystem::path s_AssetPath = (s_BasePath / "Assets" += std::filesystem::path("\\").make_preferred()).make_preferred();
		static inline const std::filesystem::path s_SciptPath = (s_BasePath / "Scripts" += std::filesystem::path("\\").make_preferred()).make_preferred();

		static inline const std::filesystem::path s_DefaultTexturePath = (s_AssetPath / "default.png").make_preferred();
		static inline const std::filesystem::path s_DefaultFontPath = (s_AssetPath / "default.ttf").make_preferred();
	private:
		// std::any for no specific reason
		static inline std::unordered_map<std::string, std::any> s_Assets;
	};
}
