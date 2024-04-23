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
	m_pScoreChanged{std::make_unique<dae::Subject<ScoreUIComponent>>()},
    m_pVecObservedSubjects{}
{
	m_pScoreChanged->AddObserver(pObserver);	
}
ScoreUIComponent::~ScoreUIComponent()
{
    for (auto& pSubject : m_pVecObservedSubjects)
    {
        pSubject->RemoveObserver(this);
    }
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

void ScoreUIComponent::AddSubjectPointer(dae::Subject<PickUpComponent>* pSubject)
{
    m_pVecObservedSubjects.emplace_back(pSubject);
}

int ScoreUIComponent::GetScore() const
{
	return m_Score;
}

