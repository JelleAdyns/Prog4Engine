#include <SDL_ttf.h>
#include "TextComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Font.h"

namespace dae
{
	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, const std::shared_ptr<Font>& font)
		: Component{pOwner}, m_needsUpdate{true}, m_text{text}, m_pFont{font}, m_pTextTexture{nullptr}, m_pRenderComponent{nullptr}
	{
	
	}


	void TextComponent::Update()
	{
		if (!m_pRenderComponent)
		{
			using ThisType = std::remove_reference<decltype(*this)>::type;
			m_pRenderComponent = GetOwner()->GetComponent<RenderComponent>();
			m_pRenderComponent->AddTexture<ThisType>(m_pTextTexture);
		}
		if (m_needsUpdate)
		{
			m_pTextTexture = ResourceManager::GetInstance().LoadTextureFromFont(m_text, m_pFont);

			using ThisType = std::remove_reference<decltype(*this)>::type;
			m_pRenderComponent->AddTexture<ThisType>(m_pTextTexture);

			m_needsUpdate = false;
		}
	}


	// This implementation uses the "dirty flag" pattern
	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}
	/*std::shared_ptr<Texture2D> TextComponent::GetTexture() const
	{
		return m_pTextTexture;
	}*/

}
	
