#include "ScoreUIComponent.h"
#include "PickUpComponent.h"
#include "TextComponent.h"
#include "Achievements.h"
#include "GameObject.h"


ScoreUIComponent::ScoreUIComponent(dae::GameObject* pOwner, Achievements* pObserver):
	dae::Component{pOwner},
	Observer{},
	m_Score{},
	m_pTextComponent{},
	m_pScoreChanged{std::make_unique<dae::Subject<ScoreUIComponent>>()}
{
	m_pScoreChanged->AddObserver(pObserver);	
}
void ScoreUIComponent::Update()
{
	if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();
}
void ScoreUIComponent::PrepareImGuiRender()
{
}
void ScoreUIComponent::Notify(PickUpComponent* pSubject)
{
	switch (pSubject->GetPickUpType())
	{
	case PickUpComponent::PickUpType::Melon:
		m_Score += 50;
		break;
	case PickUpComponent::PickUpType::Fries:
		m_Score += 100;
		break;
	}
	m_pScoreChanged->NotifyObservers(this);
	m_pTextComponent->SetText("Score: " + std::to_string(m_Score));
}

int ScoreUIComponent::GetScore() const
{
	return m_Score;
}

