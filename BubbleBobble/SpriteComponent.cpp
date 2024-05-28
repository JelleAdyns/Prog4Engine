#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include <RenderComponent.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <GameTime.h>

SpriteComponent::SpriteComponent(dae::GameObject* pOwner, const std::string& texturePath, int nrCols, int nrRows, float frameTime, bool needsUpdate, bool needsRowUpdate) :
	SpriteComponent{pOwner, std::move(dae::ResourceManager::GetInstance().LoadTexture(texturePath)), nrCols, nrRows, frameTime, needsUpdate, needsRowUpdate}
{

}
SpriteComponent::SpriteComponent(dae::GameObject* pOwner, std::unique_ptr<dae::Texture2D>&& pTexture, int nrCols, int nrRows, float frameTime, bool needsUpdate, bool needsRowUpdate):
	dae::Component{ pOwner },
	m_SpriteIsDirty{false},
	m_NeedsUpdate{ needsUpdate },
	m_NeedsRowUpdate{ needsRowUpdate },
	m_IsLookingLeft{ false },
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


	//m_pRowFinished->AddObserver(pObserver);

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

void SpriteComponent::Start()
{
	if (!m_pRenderComponent)
	{
		using ThisType = std::remove_reference<decltype(*this)>::type;
		m_pRenderComponent = GetOwner()->GetComponent<dae::RenderComponent>();
		m_pRenderComponent->AddTexture<ThisType>(m_pTexture);
	}
}

void SpriteComponent::Update()
{
	

	if (m_SpriteIsDirty) UpdateSrcRect();

	if(m_NeedsUpdate)
	{
		m_PassedTime += dae::GameTime::GetInstance().GetDeltaTime();
		if (m_PassedTime >= m_FrameTime)
		{
			
			if (m_CurrentCol == m_NrOfCols - 1)
			{
				m_pRowFinished->NotifyObservers(this);
				if (m_NeedsRowUpdate) ++m_CurrentRow %= m_NrOfRows;
			}

 			++m_CurrentCol %= m_NrOfCols;
			m_PassedTime = 0.f;

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
	m_SpriteIsDirty = true;
}
void SpriteComponent::SetRow(int row)
{
	m_CurrentRow = row;
	m_SpriteIsDirty = true;
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

	m_SpriteIsDirty = true;
}

void SpriteComponent::SetNrOfRows(int nrOfRows, bool updateSrcRect)
{
	m_NrOfRows = nrOfRows;
	if (updateSrcRect) m_SpriteIsDirty = true;
}

void SpriteComponent::SetNrOfCols(int nrOfCols, bool updateSrcRect)
{
	m_NrOfCols = nrOfCols;
	if(updateSrcRect) m_SpriteIsDirty = true;
}

void SpriteComponent::SetRowUpdate(bool rowNeedsToUpdate)
{
	m_NeedsRowUpdate = rowNeedsToUpdate;
}

void SpriteComponent::SetUpdate(bool needsToUpdate)
{
	m_NeedsUpdate = needsToUpdate;
}

void SpriteComponent::SetFrameTime(float frameTime)
{
	m_FrameTime = frameTime;
}

void SpriteComponent::LookLeft(bool isLookingLeft)
{
	m_IsLookingLeft = isLookingLeft;

	using ThisType = std::remove_reference<decltype(*this)>::type;
	if(m_pRenderComponent) m_pRenderComponent->SetFlipped<ThisType>(m_IsLookingLeft);
}

void SpriteComponent::Flip()
{
	m_IsLookingLeft = !m_IsLookingLeft;

	using ThisType = std::remove_reference<decltype(*this)>::type;
	if (m_pRenderComponent) m_pRenderComponent->SetFlipped<ThisType>(m_IsLookingLeft);
}

void SpriteComponent::AddRows(int nrOfRowsToAdd)
{
	m_CurrentRow += nrOfRowsToAdd;
#ifndef NDEBUG
	if (m_CurrentRow < 0) std::cout << "CurrentRow is smaller than 0.\n";
	if (m_CurrentRow >= m_NrOfRows) std::cout << "CurrentRow is greater than max.\n";
#endif // !NDEBUG

}

void SpriteComponent::AddObserver(dae::Observer<SpriteComponent>* pObserver)
{
	m_pRowFinished->AddObserver(pObserver);
}

bool SpriteComponent::IsLookingLeft() const
{
	return m_IsLookingLeft;
}

int SpriteComponent::GetCurrRow() const
{
	return m_CurrentRow;
}

glm::ivec2 SpriteComponent::GetTextureSize() const
{
	return m_pTexture->GetTextureSize();
}

glm::ivec2 SpriteComponent::GetDestRectSize() const
{
	return glm::ivec2{m_pTexture->GetDstRect().w, m_pTexture->GetDstRect().h};
}

void SpriteComponent::UpdateSrcRect()
{

	m_pTexture->SetSrcRect(
		glm::ivec2{ static_cast<float>(m_pTexture->GetSrcRect().w) * m_CurrentCol,
		m_StartHeightMarker + (m_EndHeightMarker - m_StartHeightMarker) / m_NrOfRows * m_CurrentRow },
		static_cast<float>(m_pTexture->GetSrcRect().w),
		(m_EndHeightMarker - m_StartHeightMarker) / m_NrOfRows);

	m_pTexture->SetDstRect(
		GetOwner()->GetWorldPosition(),
		static_cast<float>(m_pTexture->GetSrcRect().w),
		static_cast<float>(m_pTexture->GetSrcRect().h)
	);

	m_SpriteIsDirty = false;

}
