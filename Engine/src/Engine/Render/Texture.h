#pragma once

#include "Engine/Core/Base.h"

#include <glm/glm.hpp>

struct SDL_Texture;

namespace tsEngine
{
	class Renderer;

	struct Texture
	{
	private:
		Scope2<SDL_Texture> Tex;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	public:
		std::string Path;

		Texture() = default;
		Texture(Renderer* renderer, const std::string& path);
		// NOTE: Plain texture (use as render target)
		Texture(Renderer* renderer, uint32_t width, uint32_t height);

		~Texture();

		static Ref<Texture> Create(Renderer* renderer, const std::string& path);
		static Ref<Texture> Create(Renderer* renderer, uint32_t width, uint32_t height);

		// Helper functions
		
		SDL_Texture* Raw() const;
		glm::vec2 Size() const;
	};
}
