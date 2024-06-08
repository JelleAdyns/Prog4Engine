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
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, bool flip) const;
		void RenderTexture(const Texture2D& texture, SDL_Rect srcRect, SDL_Rect dstRect, bool flip) const;
		void RenderTranslucentBackGround() const;

		SDL_Renderer* GetSDLRenderer() const;
		
		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

		void StartFadeIn(float mulptiplier);

		void SetRenderBG(bool renderBG);
		bool NeedsToRenderBG() const { return m_RenderBG; }

	private:

		void FadeIn();

		friend class Singleton<Renderer>;
		Renderer() = default;
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};	
		SDL_Rect m_WindowRect{};	

		bool m_RenderBG{};

		float m_FadeTimer{};
		float m_FadeMultiplier{};
		
		bool m_FadingIn{};
		float m_AlphaVaule{};
	};
}
#endif // !RENDERER_H