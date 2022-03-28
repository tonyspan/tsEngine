#include "pch.h"

#include "Asset.h"

#include "Engine/Log/Log.h"

#include <SDL.h>
#include <SDL_ttf.h>

namespace tsEngine
{
	Asset::Asset(const AssetType& type, const std::string& path)
		: m_AssetType(type), m_Path(path)
	{
	}
	
	const AssetType Asset::GetType() const
	{
		return m_AssetType;
	}
	
	const std::string& Asset::GetPath() const
	{
		return m_Path;
	}
}