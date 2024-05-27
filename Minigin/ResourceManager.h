#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <map>
#include <memory>
#include <iostream>
#include "Singleton.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	/*class Texture2D;
	class Font;*/
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:

		virtual ~ResourceManager();

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;
		ResourceManager& operator= (const ResourceManager&) = delete;
		ResourceManager& operator= (ResourceManager&&) noexcept = delete;

		void Init(const std::string& data);

		std::unique_ptr<Texture2D>LoadTexture(const std::string& file);
		std::unique_ptr<Texture2D> LoadTextureFromFont(const std::string& text, const std::unique_ptr<Font>& font) const;
		std::unique_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		const std::string& GetDataPath() const { return m_DataPath; }

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;

		struct SDLTextureDeleter
		{
			void operator()(SDL_Texture* pTexture) { SDL_DestroyTexture(pTexture);}
		};
		std::map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> m_pMapSDLTextures;
	};
}
#endif // !RESOURCEMANAGER_H