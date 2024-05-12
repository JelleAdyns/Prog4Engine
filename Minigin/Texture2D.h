#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <SDL.h>
#include <glm/vec2.hpp>

namespace dae
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture, bool usesFont);
		~Texture2D();

		Texture2D(const Texture2D& other) = delete;
		Texture2D(Texture2D&& other) noexcept:
			m_Texture{ other.m_Texture },
			m_SrcRect {other.m_SrcRect},
			m_DstRect {other.m_DstRect},
			m_UsesFont {other.m_UsesFont}
		{
			other.m_Texture = nullptr;
		};
		Texture2D& operator= (const Texture2D& other) = delete;
		Texture2D& operator= (Texture2D&& other) noexcept
		{
			if (&other != this)
			{
				m_Texture = other.m_Texture;
				other.m_Texture = nullptr;
				m_SrcRect = other.m_SrcRect;
				m_DstRect = other.m_DstRect;
				m_UsesFont = other.m_UsesFont;
			}
			return *this;
		};

		void SetSrcRect(const glm::ivec2& offset, const float width, const float height);
		void SetSrcRect(const glm::ivec2& offset);
		void SetDstRect(const glm::ivec2& pos, const float width, const float height);
		void SetDstRect(const glm::ivec2& pos);

		glm::ivec2 GetTextureSize() const;

		SDL_Rect GetSrcRect() const;
		SDL_Rect GetDstRect() const;
		bool UsesFont() const { return m_UsesFont; }


	private:
		SDL_Texture* m_Texture;

		SDL_Rect m_SrcRect;
		SDL_Rect m_DstRect;
		bool m_UsesFont{ false };
	};
}
#endif // !TEXTURE2D_H