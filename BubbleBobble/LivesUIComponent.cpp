#include "LivesUIComponent.h"
#include "TextComponent.h"
#include "LivesComponent.h"
#include "GameObject.h"

LivesUIComponent::LivesUIComponent(dae::GameObject* pOwner) :
	dae::Component{pOwner},
	Observer{},
	m_pTextComponent{}
{
}

void LivesUIComponent::Update()
{
	if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();
}

void LivesUIComponent::PrepareImGuiRender()
{
}

void LivesUIComponent::Notify(LivesComponent* pSubject)
{
	m_pTextComponent->SetText("Remaining lives: " + std::to_string(pSubject->GetNrOfLives()));
}