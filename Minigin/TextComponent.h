#pragma once
#include <string>
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;
	class RenderComponent;

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(GameObject* pOwner, const std::string& text, const std::shared_ptr<Font>& font);
		virtual ~TextComponent() {}

		TextComponent(const TextComponent&) = delete;
		TextComponent(TextComponent&&) noexcept = delete;
		TextComponent& operator= (const TextComponent&) = delete;
		TextComponent& operator= (TextComponent&&) noexcept = delete;

		virtual void Update() override;
	

		void SetText(const std::string& text);
		//std::shared_ptr<Texture2D> GetTexture() const;

	private:
		bool m_needsUpdate;
		std::string m_text;

		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture;

		RenderComponent* m_pRenderComponent;
	};
}
