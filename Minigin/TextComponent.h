#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <string>
#include "Component.h"
#include "Font.h"
#include "Texture2D.h"
#include "glm/vec4.hpp"

namespace dae
{
	class RenderComponent;

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(GameObject* pOwner, const std::string& text, const std::string& fontFile, unsigned int size, const glm::u8vec4& color = {255,255,255,255});
		explicit TextComponent(GameObject* pOwner, const std::string& text, std::unique_ptr<Font>&& pFont, std::unique_ptr<Texture2D>&& pTexture, const glm::u8vec4& color = { 255,255,255,255 });
		explicit TextComponent(GameObject* pOwner, const std::string& text, std::unique_ptr<Font>&& pFont, const glm::u8vec4& color = { 255,255,255,255 });
		virtual ~TextComponent() {}

		TextComponent(const TextComponent&) = delete;
		TextComponent(TextComponent&&) noexcept = delete;
		TextComponent& operator= (const TextComponent&) = delete;
		TextComponent& operator= (TextComponent&&) noexcept = delete;

		virtual void Start() override;
		virtual void Update() override;
		virtual void PrepareImGuiRender() override;


		void SetText(const std::string& text);
		Texture2D* GetTexture() const;

	private:
		bool m_needsUpdate;
		std::string m_text;

		std::unique_ptr<Font> m_pFont;
		std::unique_ptr<Texture2D> m_pTextTexture;
		 
		SDL_Color m_Color;

		RenderComponent* m_pRenderComponent;
	};
}
#endif // !TEXTCOMPONENT_H