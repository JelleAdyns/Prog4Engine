#include <stdexcept>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "Font.h"

namespace dae
{
	TTF_Font* Font::GetFont() const {
		return m_font;
	}

	Font::Font(const std::string& fullPath, unsigned int size) : m_font(nullptr)
	{
		auto scale = Minigin::GetWindowScale();
		m_font = TTF_OpenFont(fullPath.c_str(), size * scale);
		if (m_font == nullptr) 
		{
			throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
		}
	}

	Font::~Font()
	{
		TTF_CloseFont(m_font);
	}
}

