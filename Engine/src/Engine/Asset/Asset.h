#pragma once

#include "Engine/Core/Base.h"

struct SDL_Texture;
typedef struct _TTF_Font TTF_Font;

namespace tsEngine
{
	enum class AssetType
	{
		Undefined = 0,
		Texture,
		Font,
		//Music,
		//Sfx,
		Workspace
	};

	struct SupportedAsset
	{
		bool IsSupported = false;
		AssetType Type = AssetType::Undefined;

		operator bool() const { return IsSupported; }
	};

	class Asset
	{
	public:
		Asset(const AssetType& type = AssetType::Undefined, const std::string& path = std::string());
		virtual ~Asset() = default;

		const AssetType GetType() const;
		const std::string& GetPath() const;

		// Dangerous/weird way to do it?
		// TODO: Update static_assert for each new subclass of Asset or improve it
		template<typename T>
		T* As() const
		{
			static_assert(std::is_same_v<T, SDL_Texture> || std::is_same_v<T, TTF_Font>);
			return static_cast<T*>(Raw());
		}

		template<typename T, typename... Args>
		static Ref<T> Create(Args&&... args)
		{
			return CreateRef<T>(std::forward<Args>(args)...);
		}

		static SupportedAsset IsSupported(const std::filesystem::path& path)
		{
			auto found = s_SupportedFormats.find(path.extension().string());
			return { found != s_SupportedFormats.end(), found->second };
		}
	protected:
		virtual void* Raw() const = 0;
	protected:
		AssetType m_AssetType;
		std::string m_Path;
	private:
		using SupportedFormatsMap = std::unordered_map<std::string, AssetType>;
		static inline SupportedFormatsMap s_SupportedFormats{ { ".jpg", AssetType::Texture },
															  { ".png", AssetType::Texture },
															  { ".ttf", AssetType::Font },
															  { ".tse", AssetType::Workspace } };

		friend class AssetManager;
	};
}