#pragma once
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
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		//void AddRenderComp(const std::shared_ptr<dae::RenderComponent>& pRenderComp)
		//{
		//	m_pVecRenderComps.emplace_back(pRenderComp);
		//}

	private:
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	

		//std::vector<std::shared_ptr<dae::RenderComponent>> m_pVecRenderComps;
	};
}

