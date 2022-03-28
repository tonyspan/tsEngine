#include "pch.h"

#include "Font.h"

#include "Engine/Log/Log.h"

//#include <SDL.h>
#include <SDL_ttf.h>

namespace tsEngine
{
	Font::Font()
		: Asset(AssetType::Font)
	{
	}

	Font::Font(const std::string& path, int fontSize)
		: Asset(AssetType::Font, path), FontSize(fontSize)
	{
		FFont = TTF_OpenFont(path.c_str(), fontSize);

		if (!FFont)
			LOG_ERROR("Unable to open font, SDL_Image returned error {}", TTF_GetError());
	}

	Font::~Font()
	{
		Close();
	}

	void Font::Close()
	{
		if (!FFont)
			TTF_CloseFont(FFont);
	}
}
