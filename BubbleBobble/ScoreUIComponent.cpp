#include "ScoreUIComponent.h"
#include "PickUpComponent.h"
#include "TextComponent.h"
#include "InventoryComponent.h"
#include "PlayerComponent.h"
#include "Achievements.h"
#include "Spawners.h"
#include "GameObject.h"
#include <algorithm>


ScoreUIComponent::ScoreUIComponent(dae::GameObject* pOwner, int startScore, Achievements* pObserver):
	dae::Component{pOwner},
	dae::Observer<InventoryComponent>{},
	dae::Observer<PlayerComponent>{},
	m_TotalScore{startScore},
	m_pTextComponent{},
	m_pScoreChanged{std::make_unique<dae::Subject<ScoreUIComponent>>()},
    m_pVecObservedInventorySubject{}
{
	m_pScoreChanged->AddObserver(pObserver);	
}
ScoreUIComponent::~ScoreUIComponent()
{
    if(m_pVecObservedInventorySubject) m_pVecObservedInventorySubject->RemoveObserver(this);
    if(m_pVecObservedPlayerSubject) m_pVecObservedPlayerSubject->RemoveObserver(this);
}

void ScoreUIComponent::Start()
{
	if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();
	m_pTextComponent->SetText(std::to_string(m_TotalScore));
}

void ScoreUIComponent::Update()
{
	
}
void ScoreUIComponent::PrepareImGuiRender()
{
}
void ScoreUIComponent::Notify(InventoryComponent* pSubject)
{
	auto [pickUpType, pickUpPos] = pSubject->GetLastAddedItem();
	int earnedsScore{};
	switch (pickUpType)
	{
	case PickUpComponent::PickUpType::Melon:
		earnedsScore = 100;
		break;
	case PickUpComponent::PickUpType::Fries:
		earnedsScore = 200;
		break;
	}

	m_TotalScore += earnedsScore;
	spawners::SpawnFloatingScore(pickUpPos, earnedsScore, pSubject->GetPlayerType());

	m_pScoreChanged->NotifyObservers(this);
	m_pTextComponent->SetText( std::to_string(m_TotalScore));
}
void ScoreUIComponent::AddSubjectPointer(dae::Subject<InventoryComponent>* pSubject)
{
	m_pVecObservedInventorySubject = pSubject;
}

void ScoreUIComponent::SetSubjectPointersInvalid(dae::Subject<InventoryComponent>*)
{
	m_pVecObservedInventorySubject = nullptr;
}
void ScoreUIComponent::Notify(PlayerComponent* pSubject)
{
	//Player died

	if (pSubject->GetNrOfLives() == 0) m_TotalScore -= 300;
	else m_TotalScore -= 150;

	if (m_TotalScore < 0) m_TotalScore = 0;

	m_pScoreChanged->NotifyObservers(this);
	m_pTextComponent->SetText(std::to_string(m_TotalScore));
}

void ScoreUIComponent::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedPlayerSubject = pSubject;
}

void ScoreUIComponent::SetSubjectPointersInvalid(dae::Subject<PlayerComponent>*)
{
	m_pVecObservedPlayerSubject = nullptr;
}

int ScoreUIComponent::GetScore() const
{
	return m_TotalScore;
}

