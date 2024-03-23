#include "ScoreUIComponent.h"
#include "PickUpComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

namespace dae
{
	ScoreUIComponent::ScoreUIComponent(GameObject* pOwner):
		Component{pOwner},
		Observer{},
		m_pTextComponent{},
		m_Score{}
	{
	}
	void ScoreUIComponent::Update()
	{
		if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
	}
	void ScoreUIComponent::PrepareImGuiRender()
	{
	}
	void ScoreUIComponent::Notify(PickUpComponent* pSubject)
	{
		m_Score += pSubject->GetScoreValue();
		m_pTextComponent->SetText("Score: " + std::to_string(m_Score));
	}
}
