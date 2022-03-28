#include "pch.h"

#include "Texture.h"
#include "Renderer.h"

#include "Engine/Log/Log.h"

#include <SDL.h>
#include <SDL_image.h>

namespace tsEngine
{
	void Deleter2<SDL_Texture>::operator()(SDL_Texture* ptr)
	{
		SDL_DestroyTexture(ptr);
	}

	Texture::Texture()
		: Asset(AssetType::Texture)
	{
	}

	Texture::Texture(const std::string& path)
		: Asset(AssetType::Texture, path)
	{
		Tex.reset(IMG_LoadTexture(Renderer::s_Instance->GetNativeRenderer(), path.c_str()));

		if (!Tex)
			LOG_ERROR("Unable to load: {}, SDL_Image returned error {}", path, IMG_GetError());
	}

	Texture::Texture(uint32_t width, uint32_t height)
	{
		Tex.reset(SDL_CreateTexture(Renderer::s_Instance->GetNativeRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height));

		if (!Tex)
			LOG_ERROR("Unable to create texture, SDL_Image returned error {}", IMG_GetError());
	}

	Texture::~Texture()
	{
		Tex.reset();
	}

	glm::vec2 Texture::Size() const
	{
		SDL_QueryTexture(Tex.get(), NULL, NULL, (int*)(uint32_t*)&m_Width, (int*)(uint32_t*)&m_Height);

		return glm::vec2{ m_Width, m_Height };
	}

}
