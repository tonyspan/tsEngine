#pragma once

#include "Engine/Core/Base.h"

#include <glm/glm.hpp>

struct SDL_Renderer;
typedef struct _TTF_Font TTF_Font;

namespace tsEngine
{
	class Renderer;

	struct Font
	{
	private:
		Scope2<TTF_Font> FFont;
	public:
		// For ease of use
		std::string Path;
		int FontSize;

		Font() = default;
		Font(const std::string& path, int fontSize);

		~Font();

		static Ref<Font> Create(const std::string& path, int fontSize);

		// Helper functions

		TTF_Font* Raw() const;
		void Close();
	};
}
