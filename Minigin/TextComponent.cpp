#include <SDL_ttf.h>
#include "TextComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"

namespace dae
{
	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, const std::string& fontFile, unsigned int size)
		: Component{pOwner},
		m_needsUpdate{true},
		m_text{text},
		m_pFont{ResourceManager::GetInstance().LoadFont(fontFile, size)},
		m_pTextTexture{nullptr},
		m_pRenderComponent{nullptr}
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
			m_pTextTexture->SetDstRect(GetOwner()->GetWorldPosition());

			m_needsUpdate = false;
		}
	}

	void TextComponent::PrepareImGuiRender() {}


	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}

	Texture2D* TextComponent::GetTexture() const
	{
		return m_pTextTexture.get();
	}

}
	
