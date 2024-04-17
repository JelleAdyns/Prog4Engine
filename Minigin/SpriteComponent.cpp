#include "SpriteComponent.h"
#include <RenderComponent.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <GameTime.h>
namespace dae
{
	SpriteComponent::SpriteComponent(GameObject* pOwner, const std::string& texturePath, int nrCols, int nrFrames, float frameTime, bool needsToUpdate):
		Component{pOwner}, 
		m_NeedsUpdate{needsToUpdate},
		m_IsLookingLeft{false},
		m_CurrentCol{0},
		m_CurrentRow{0},
		m_NrOfCols{nrCols},
		m_NrOfFrames{nrFrames},
		m_NrOfRows{nrFrames/nrCols},
		m_FrameTime{frameTime},
		m_PassedTime{0.f},
		m_pRenderComponent{ nullptr },
		m_pTexture {ResourceManager::GetInstance().LoadTexture(texturePath)} 
	{
		m_pTexture->SetSrcRect(
			glm::ivec2{},
			static_cast<float>(m_pTexture->GetTextureSize().x / m_NrOfCols),
			static_cast<float>(m_pTexture->GetTextureSize().y / m_NrOfRows)
		);

		m_pTexture->SetDstRect(
			glm::ivec2{},
			static_cast<float>(m_pTexture->GetSrcRect().w),
			static_cast<float>(m_pTexture->GetSrcRect().h)
		);
	}
	void SpriteComponent::Update()
	{
		if (!m_pRenderComponent)
		{
			using ThisType = std::remove_reference<decltype(*this)>::type;
			m_pRenderComponent = GetOwner()->GetComponent<RenderComponent>();
			m_pRenderComponent->AddTexture<ThisType>(m_pTexture);
		}

		if (m_NeedsUpdate)
		{
			m_PassedTime += GameTime::GetInstance().GetDeltaTime();
			if (m_PassedTime >= m_FrameTime)
			{
				if (m_CurrentCol == m_NrOfCols - 1) m_CurrentRow %= m_NrOfRows;
				++m_CurrentCol %= m_NrOfCols;
				m_PassedTime -= m_FrameTime;

				m_pTexture->SetSrcRect(
					glm::ivec2{ static_cast<float>(m_pTexture->GetTextureSize().x) / m_NrOfCols * m_CurrentCol,
					static_cast<float>(m_pTexture->GetTextureSize().y) / m_NrOfRows * m_CurrentRow });
			}
		}
	}
	void SpriteComponent::PrepareImGuiRender()
	{
	}
	void SpriteComponent::SetUpdate(bool needsToUpdate)
	{
		m_NeedsUpdate = needsToUpdate;
	}
	void SpriteComponent::SetFrame(int col, int row)
	{
		m_CurrentCol = col;
		m_CurrentRow = row;

		m_pTexture->SetSrcRect(
			glm::ivec2{ static_cast<float>(m_pTexture->GetTextureSize().x) / m_NrOfCols * m_CurrentCol,
			static_cast<float>(m_pTexture->GetTextureSize().y) / m_NrOfRows * m_CurrentRow });
	}
}