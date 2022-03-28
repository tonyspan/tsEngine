#pragma once

#include "Engine/Core/Base.h"

#include "Engine/Asset/Asset.h"

typedef struct _TTF_Font TTF_Font;

namespace tsEngine
{
	class Renderer;

	class Font : public Asset
	{
	public:
		Font();
		Font(const std::string& path, int fontSize);
		~Font();

		void Close();
	protected:
		void* Raw() const override { return FFont; }
	public:
		// For ease of use
		int FontSize;
	private:
		TTF_Font* FFont = nullptr;
	};
}
