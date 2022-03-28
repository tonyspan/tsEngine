#pragma once

#include "Engine/Core/Base.h"

#include "Engine/Asset/Asset.h"

#include <glm/glm.hpp>

struct SDL_Texture;

namespace tsEngine
{
	class Renderer;

	class Texture : public Asset
	{
	public:
		Texture();
		Texture(const std::string& path);
		// NOTE: Plain texture (use as render target)
		Texture(uint32_t width, uint32_t height);
		~Texture();

		glm::vec2 Size() const;
	protected:
		void* Raw() const override { return Tex.get(); }
	private:
		Scope2<SDL_Texture> Tex;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}
