#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include <RenderComponent.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <GameTime.h>

SpriteComponent::SpriteComponent(dae::GameObject* pOwner, const std::string& texturePath, int nrCols, int nrRows, float frameTime, bool needsUpdate, bool needsRowUpdate, PlayerComponent* pObserver) :
	SpriteComponent{pOwner, std::move(dae::ResourceManager::GetInstance().LoadTexture(texturePath)), nrCols, nrRows, frameTime, needsUpdate, needsRowUpdate, pObserver}
{

}
SpriteComponent::SpriteComponent(dae::GameObject* pOwner, std::unique_ptr<dae::Texture2D>&& pTexture, int nrCols, int nrRows, float frameTime, bool needsUpdate, bool needsRowUpdate, PlayerComponent* pObserver):
	dae::Component{ pOwner },
	m_NeedsUpdate{ needsUpdate },
	m_NeedsRowUpdate{ needsRowUpdate },
	m_IsLookingLeft{ false },
	m_StartRow{0},
	m_CurrentCol{ 0 },
	m_CurrentRow{ 0 },
	m_NrOfCols{ nrCols },
	m_NrOfRows{ nrRows },
	m_FrameTime{ frameTime },
	m_PassedTime{ 0.f },
	m_StartHeightMarker{ 0 },
	m_EndHeightMarker{ 0 },
	m_pRenderComponent{ nullptr },
	m_pTexture{ std::move(pTexture) },
	m_pRowFinished{std::make_unique<dae::Subject<SpriteComponent>>()}
{

	m_pRowFinished->AddObserver(pObserver);

	m_EndHeightMarker = static_cast<float>(m_pTexture->GetTextureSize().y);

	m_pTexture->SetSrcRect(
		glm::ivec2{},
		static_cast<float>(m_pTexture->GetTextureSize().x / m_NrOfCols),
		static_cast<float>((m_EndHeightMarker - m_StartHeightMarker) / m_NrOfRows)
	);

	m_pTexture->SetDstRect(
		GetOwner()->GetWorldPosition(),
		static_cast<float>(m_pTexture->GetSrcRect().w),
		static_cast<float>(m_pTexture->GetSrcRect().h)
	);
}

void SpriteComponent::Update()
{
	if (!m_pRenderComponent)
	{
		using ThisType = std::remove_reference<decltype(*this)>::type;
		m_pRenderComponent = GetOwner()->GetComponent<dae::RenderComponent>();
		m_pRenderComponent->AddTexture<ThisType>(m_pTexture);
	}

	if(m_NeedsUpdate)
	{
		m_PassedTime += dae::GameTime::GetInstance().GetDeltaTime();
		if (m_PassedTime >= m_FrameTime)
		{
			
			if (m_CurrentCol == m_NrOfCols - 1)
			{

				m_pRowFinished->NotifyObservers(this);
				if (m_NeedsRowUpdate) m_CurrentRow %= m_NrOfRows + m_StartRow;
			}

 			++m_CurrentCol %= m_NrOfCols;
			m_PassedTime -= m_FrameTime;

			UpdateSrcRect();
		}
	}
	
}
void SpriteComponent::PrepareImGuiRender()
{
}
void SpriteComponent::SetCol(int col)
{
	m_CurrentCol = col;
	UpdateSrcRect();
}
void SpriteComponent::SetRow(int row)
{
	m_CurrentRow = m_StartRow + row;
	UpdateSrcRect();
}

void SpriteComponent::SetHeightMarkers(float startHeight, float endHeight)
{
	m_StartHeightMarker = startHeight;
	m_EndHeightMarker = endHeight;

	m_pTexture->SetSrcRect(
		glm::vec2{ m_pTexture->GetSrcRect().x, m_pTexture->GetSrcRect().y },
		static_cast<float>(m_pTexture->GetTextureSize().x) / m_NrOfCols,
		(m_EndHeightMarker - m_StartHeightMarker) / m_NrOfRows );

	m_pTexture->SetDstRect(
		GetOwner()->GetWorldPosition(),
		static_cast<float>(m_pTexture->GetSrcRect().w),
		static_cast<float>(m_pTexture->GetSrcRect().h)
	);
}

void SpriteComponent::SetNrOfRows(int nrOfRows)
{
	m_NrOfRows = nrOfRows;
}

void SpriteComponent::SetRowUpdate(bool rowNeedsToUpdate)
{
	m_NeedsRowUpdate = rowNeedsToUpdate;
}

void SpriteComponent::SetStartRow(int startRow)
{
	m_StartRow = startRow;
	m_CurrentRow -= (m_CurrentRow - m_StartRow);
	UpdateSrcRect();
}

void SpriteComponent::SetFrameTime(float frameTime)
{
	m_FrameTime = frameTime;
}

void SpriteComponent::LookLeft(bool isLookingLeft)
{
	m_IsLookingLeft = isLookingLeft;

	using ThisType = std::remove_reference<decltype(*this)>::type;
	m_pRenderComponent->SetFlipped<ThisType>(m_IsLookingLeft);
}

glm::ivec2 SpriteComponent::GetTextureSize() const
{
	return m_pTexture->GetTextureSize();
}

void SpriteComponent::UpdateSrcRect() const
{
	m_pTexture->SetSrcRect(
		glm::ivec2{ static_cast<float>(m_pTexture->GetTextureSize().x) / m_NrOfCols * m_CurrentCol,
		(m_EndHeightMarker - m_StartHeightMarker) / m_NrOfRows * m_CurrentRow });
}
