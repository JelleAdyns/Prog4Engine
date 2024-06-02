#include "ScoreUIComponent.h"
#include "PickUpComponent.h"
#include "TextComponent.h"
#include "InventoryComponent.h"
#include "Achievements.h"
#include "Spawners.h"
#include "GameObject.h"


ScoreUIComponent::ScoreUIComponent(dae::GameObject* pOwner, Achievements* pObserver):
	dae::Component{pOwner},
	Observer{},
	m_TotalScore{},
	m_pTextComponent{},
	m_pScoreChanged{std::make_unique<dae::Subject<ScoreUIComponent>>()},
    m_pVecObservedSubjects{}
{
	m_pScoreChanged->AddObserver(pObserver);	
}
ScoreUIComponent::~ScoreUIComponent()
{
    for (auto& pSubject : m_pVecObservedSubjects)
    {
        if(pSubject) pSubject->RemoveObserver(this);
    }
}

void ScoreUIComponent::Start()
{
	if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();
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
    m_pVecObservedSubjects.emplace_back(pSubject);
}

void ScoreUIComponent::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		pSubject = nullptr;
	}
}

int ScoreUIComponent::GetScore() const
{
	return m_TotalScore;
}

