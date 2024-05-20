#include <SDL_ttf.h>
#include "TextComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"

namespace dae
{
	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, const std::string& fontFile, unsigned int size)
		: Component{pOwner},
		m_needsUpdate{false},
		m_text{text},
		m_pFont{ResourceManager::GetInstance().LoadFont(fontFile, size)},
		m_pTextTexture{nullptr},
		m_pRenderComponent{nullptr}
	{
	
	}

	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::unique_ptr<Font>&& pFont, std::unique_ptr<Texture2D>&& pTexture)
		: Component{ pOwner },
		m_needsUpdate{ false },
		m_text{ text },
		m_pFont{ std::move(pFont) },
		m_pTextTexture{ std::move(pTexture) },
		m_pRenderComponent{ nullptr }
	{
	}

	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::unique_ptr<Font>&& pFont)
		: Component{ pOwner },
		m_needsUpdate{ false },
		m_text{ text },
		m_pFont{ std::move(pFont) },
		m_pTextTexture{ nullptr },
		m_pRenderComponent{ nullptr } 
	{
	}


	void TextComponent::Start()
	{
		if (!m_pRenderComponent)
		{
			using ThisType = std::remove_reference<decltype(*this)>::type;

			m_pRenderComponent = GetOwner()->GetComponent<RenderComponent>();

			m_pTextTexture = ResourceManager::GetInstance().LoadTextureFromFont(m_text, m_pFont);
			m_pRenderComponent->AddTexture<ThisType>(m_pTextTexture);
			m_pTextTexture->SetDstRect(GetOwner()->GetWorldPosition());
		}
	}

	void TextComponent::Update()
	{
		
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
	
