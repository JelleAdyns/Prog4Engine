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
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//Mattias Devred
	SDL_RenderSetVSync(m_renderer, 1);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	//std::for_each(m_pVecRenderComps.cbegin(), m_pVecRenderComps.cend(), [&](const std::shared_ptr<dae::RenderComponent>& pRenderComp) {pRenderComp->Render(); });
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().PrepareImGuiRender();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	//int scale = Minigin::GetWindowScale();

	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	//dst.x *= scale;
	//dst.y *= scale;
	//dst.w *= scale;
	//dst.h *= scale;

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	//int scale = Minigin::GetWindowScale();

	SDL_Rect dst{};
	dst.x = static_cast<int>(x /** scale*/);
	dst.y = static_cast<int>(y /** scale*/);
	dst.w = static_cast<int>(width /** scale*/);
	dst.h = static_cast<int>(height /** scale*/);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const glm::vec2& pos, const glm::vec2& srcRectOffset, const float width, const float height) const
{
	//int scale = Minigin::GetWindowScale();

	SDL_Rect dst{};
	dst.x = static_cast<int>(pos.x /** scale*/);
	dst.y = static_cast<int>(pos.y /** scale*/);
	dst.w = static_cast<int>(width /** scale*/);
	dst.h = static_cast<int>(height /** scale*/ );

	SDL_Rect src{};
	src.x = static_cast<int>(srcRectOffset.x);
	src.y = static_cast<int>(srcRectOffset.y);
	src.w = static_cast<int>(width);
	src.h = static_cast<int>(height);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, SDL_Rect srcRect, SDL_Rect dstRect) const
{
	
	int scale = Minigin::GetWindowScale();

	SDL_Rect dst{};
	dst.x = static_cast<int>(dstRect.x * scale);
	dst.y = static_cast<int>(dstRect.y * scale);
	dst.w = static_cast<int>(dstRect.w);
	dst.h = static_cast<int>(dstRect.h);

	if (!texture.UsesFont())
	{
		dst.w *= scale;
		dst.h *= scale;
	}

	//float textureWidth{ static_cast<float>(texture.GetTextureSize().x) };
	//float textureHeight{ static_cast<float>(texture.GetTextureSize().y) };

	//float textLeft{};
	//float textRight{};
	//float textTop{};
	//float textBottom{};

	//float defaultDestWidth{};
	//float defaultDestHeight{};


	//// Convert to the range [0.0, 1.0]
	//textLeft = static_cast<float>(srcRect.x) / textureWidth;
	//textRight = (static_cast<float>(srcRect.x) + static_cast<float>(srcRect.w)) / textureWidth;
	//textTop = (static_cast<float>(srcRect.y)) / textureHeight;
	//textBottom = (static_cast<float>(srcRect.y) + static_cast<float>(srcRect.h)) / textureHeight;

	//defaultDestHeight = static_cast<float>(srcRect.h);
	//defaultDestWidth = static_cast<float>(srcRect.w);
	//

	//// Determine vertex coordinates
	//float vertexLeft{ static_cast<float>(dstRect.x) };
	//float vertexBottom{ static_cast<float>(dstRect.y) + static_cast<float>(dstRect.h) };
	//float vertexRight{ static_cast<float>(dstRect.x) + static_cast<float>(dstRect.w)};
	//float vertexTop{static_cast<float>(dstRect.y)};

	//// Tell opengl which texture we will use
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//// Draw
	//glEnable(GL_TEXTURE_2D);
	//{
	//	glBegin(GL_QUADS);
	//	{
	//		glTexCoord2f(textLeft, textBottom);
	//		glVertex2f(vertexLeft, vertexBottom);

	//		glTexCoord2f(textLeft, textTop);
	//		glVertex2f(vertexLeft, vertexTop);

	//		glTexCoord2f(textRight, textTop);
	//		glVertex2f(vertexRight, vertexTop);

	//		glTexCoord2f(textRight, textBottom);
	//		glVertex2f(vertexRight, vertexBottom);
	//	}
	//	glEnd();
	//}
	//glDisable(GL_TEXTURE_2D);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
