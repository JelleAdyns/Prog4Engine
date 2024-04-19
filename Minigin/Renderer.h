#ifndef RENDERER_H
#define RENDERER_H
#include <SDL.h>
#include <vector>
#include <memory>
#include "Singleton.h"
#include "RenderComponent.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	
	public:

		virtual ~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator= (const Renderer&) = delete;
		Renderer& operator= (Renderer&&) noexcept = delete;

		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, SDL_Rect srcRect, SDL_Rect dstRect) const;

		SDL_Renderer* GetSDLRenderer() const;
		
		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

	private:
		friend class Singleton<Renderer>;
		Renderer() = default;
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	};
}
#endif // !RENDERER_H