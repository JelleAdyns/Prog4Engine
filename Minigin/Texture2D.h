#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <SDL.h>
#include <glm/vec2.hpp>

struct SDL_Texture;
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

		void SetSrcRect(const glm::ivec2& offset, const float width, const float height);
		void SetSrcRect(const glm::ivec2& offset);
		void SetDstRect(const glm::ivec2& pos, const float width, const float height);
		void SetDstRect(const glm::ivec2& pos);

		glm::ivec2 GetTextureSize() const;

		const SDL_Rect& GetSrcRect() const { return m_SrcRect; }
		const SDL_Rect& GetDstRect() const { return m_DstRect; }
		bool UsesFont() const { return m_UsesFont; }

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_texture;

		SDL_Rect m_SrcRect;
		SDL_Rect m_DstRect;
		const bool m_UsesFont{ false };
	};
}
#endif // !TEXTURE2D_H