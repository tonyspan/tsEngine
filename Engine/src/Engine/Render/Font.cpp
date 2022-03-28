#include "pch.h"

#include "Font.h"

#include "Engine/Log/Log.h"

#include <SDL.h>
#include <SDL_ttf.h>

namespace tsEngine
{
	void Deleter2<TTF_Font>::operator()(TTF_Font* ptr)
	{
		TTF_CloseFont(ptr);
	}
	
	Font::Font(const std::string& path, int fontSize)
		: Path(path), FontSize(fontSize)
	{
		FFont.reset(TTF_OpenFont(path.c_str(), fontSize));

		if (!FFont)
			LOG_ERROR("Unable to open font, SDL_Image returned error {}", TTF_GetError());
	}

	Font::~Font()
	{
		Close();
	}
	
	Ref<Font> Font::Create(const std::string& path, int fontSize)
	{
		return CreateRef<Font>(path, fontSize);
	}
	
	TTF_Font* Font::Raw() const
	{
		return FFont.get();
	}
	
	void Font::Close()
	{
		FFont.reset();
	}
}
