
#include "Texture2D.h"

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
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

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture, bool usesFont):
	m_UsesFont{usesFont}
{
	m_texture = texture;
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
