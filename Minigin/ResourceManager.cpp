#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Minigin.h"

dae::ResourceManager::~ResourceManager()
{
#ifndef NDEBUG
	std::cout << "Destroying ResourceManager with textures:" << std::endl;
	for (const auto& pair : m_pMapSDLTextures)
	{
		std::cout << "Texture: " << pair.first << " Address: " << pair.second.get() << std::endl;
	}
#endif // !NDEBUG
}

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::unique_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file)
{

 	if (!m_pMapSDLTextures.contains(file))
	{
		const auto fullPath = m_DataPath + file;
		auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());

		if (texture == nullptr) throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
		m_pMapSDLTextures[file] = std::unique_ptr<SDL_Texture, SDLTextureDeleter>(texture, m_Deleter);
		//m_pMapSDLTextures[file] = nullptr;
		//m_pMapSDLTextures.at(file).reset(texture);
	}
	
	return std::make_unique<dae::Texture2D>(m_pMapSDLTextures.at(file).get(), false);
	
}

std::unique_ptr<dae::Texture2D> dae::ResourceManager::LoadTextureFromFont(const std::string& text, const std::unique_ptr<Font>& font) const
{
	const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(font->GetFont(), text.c_str(), color);
	if (surf == nullptr) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	SDL_FreeSurface(surf);
	
	return std::make_unique<Texture2D>(texture, true);
}

std::unique_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_unique<Font>(m_DataPath + file, size);
}
