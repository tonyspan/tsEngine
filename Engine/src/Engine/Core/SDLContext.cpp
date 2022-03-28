#include "pch.h"

#include "SDLContext.h"

#include "Engine/Log/Log.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

void SDLContext::Init()
{
	LOG_INFO("Initializing SDL");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		LOG_CRITICAL("Unable to initialize SDL. SDL error: {0}", SDL_GetError());

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
		LOG_CRITICAL("Unable to initialize SDL_Image");

	if (TTF_Init() == -1)
		LOG_CRITICAL("SDL_ttf could not initialize! SDL_ttf Error: {0}", TTF_GetError());
}

void SDLContext::Shutdown()
{
	LOG_INFO("Shutting down SDL");

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
