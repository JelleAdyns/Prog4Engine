#include "Texture2D.h"
#include "Minigin.h"
#include <iostream>

dae::Texture2D::Texture2D(SDL_Texture* texture, bool usesFont) :
	m_UsesFont{ usesFont },
	m_Texture{ texture }
{

	auto size = GetTextureSize();
	m_SrcRect.x = 0;
	m_SrcRect.y = 0;
	m_SrcRect.w = size.x;
	m_SrcRect.h = size.y;

	m_DstRect.x = 0;
	m_DstRect.y = 0;
	m_DstRect.w = size.x;
	m_DstRect.h = size.y;
}


dae::Texture2D::~Texture2D()
{
	// If the SDL_Texture is created with a font, we don't save it in the resourceManager so it needs to be destroyed here.
	// The reason we don't save it in the ResourceManager is that we would have to save every texture that has a different size,text and/or color.
	// They would each need a unique key, which is just impossible.
	if (m_UsesFont)
	{
		SDL_DestroyTexture(m_Texture);
	}
}

void dae::Texture2D::SetSrcRect(const glm::ivec2& offset, const float width, const float height)
{
	m_SrcRect.x = offset.x;
	m_SrcRect.y = offset.y;
	m_SrcRect.w = static_cast<int>(width);
	m_SrcRect.h = static_cast<int>(height);
}

void dae::Texture2D::SetSrcRect(const glm::ivec2& offset)
{
	m_SrcRect.x = offset.x;
	m_SrcRect.y = offset.y;
}

void dae::Texture2D::SetDstRect(const glm::ivec2& pos, const float width, const float height)
{
	m_DstRect.x = pos.x;
	m_DstRect.y = pos.y;
	m_DstRect.w = static_cast<int>(width);
	m_DstRect.h = static_cast<int>(height);
}

void dae::Texture2D::SetDstRect(const glm::ivec2& pos)
{
	m_DstRect.x = pos.x;
	m_DstRect.y = pos.y;
}

glm::ivec2 dae::Texture2D::GetTextureSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Rect dae::Texture2D::GetSrcRect() const
{
	return m_SrcRect;
}

SDL_Rect dae::Texture2D::GetDstRect() const
{
	float scale = static_cast<float>(dae::Minigin::GetWindowScale());
	if (m_UsesFont)
	{
		return SDL_Rect
		{
			m_DstRect.x,
			m_DstRect.y,
			static_cast<int>(m_DstRect.w / scale),
			static_cast<int>(m_DstRect.h / scale)
		};
	}
	else return m_DstRect;
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}
