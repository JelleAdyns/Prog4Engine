#include <stdexcept>
#include <algorithm>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Minigin.h"
#include <imgui.h>
#include <imgui_plot.h>
#include <SDL_opengl.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <cmath>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	auto windowSize = dae::Minigin::GetWindowSize();
	auto windowScale = dae::Minigin::GetWindowScale();
	m_WindowRect.x = 0;
	m_WindowRect.y = 0;
	m_WindowRect.w = windowScale * windowSize.x;
	m_WindowRect.h = windowScale * windowSize.y;

	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//Mattias Devred
	SDL_RenderSetVSync(m_Renderer, 1);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	
	SceneManager::GetInstance().Render();
	if (m_FadingIn) FadeIn();


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().PrepareImGuiRender();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_Renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, bool flip) const
{
	int scale = Minigin::GetWindowScale();

	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	dst.x *= scale;
	dst.y *= scale;
	dst.w *= scale;
	dst.h *= scale;

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, 0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, SDL_Rect srcRect, SDL_Rect dstRect, bool flip) const
{
	
	int scale = Minigin::GetWindowScale();

	SDL_Rect dst{};
	dst.x = static_cast<int>(dstRect.x * scale);
	dst.y = static_cast<int>(dstRect.y * scale);
	dst.w = static_cast<int>(dstRect.w * scale);
	dst.h = static_cast<int>(dstRect.h * scale);


	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dst, 0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void dae::Renderer::RenderTranslucentBackGround() const
{
	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 150);
	SDL_RenderFillRect(m_Renderer, &m_WindowRect);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_Renderer; }

void dae::Renderer::StartFadeIn(float mulptiplier)
{
	m_AlphaVaule = 255;
	m_FadeTimer = 0.f;
	m_FadingIn = true;
	m_FadeMultiplier = mulptiplier;
}

void dae::Renderer::SetRenderBG(bool renderBG)
{
	m_RenderBG = renderBG;
}


void dae::Renderer::FadeIn()
{

	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, static_cast<Uint8>(m_AlphaVaule));
	SDL_RenderFillRect(m_Renderer, &m_WindowRect);
	
	m_FadeTimer += dae::GameTime::GetInstance().GetDeltaTime();
	m_AlphaVaule = std::lerp(255.f, 0.f, m_FadeTimer * m_FadeMultiplier);

	if (m_AlphaVaule <= 0)
	{
		m_FadeTimer = 0.f;
		m_AlphaVaule = 0.f;
		m_FadingIn = false;
	}

}
