#include "LivesUIComponent.h"
#include "TextComponent.h"
#include "LivesComponent.h"
#include "GameObject.h"

namespace dae
{
	LivesUIComponent::LivesUIComponent(GameObject* pOwner) :
		Component{pOwner},
		Observer{},
		m_pTextComponent{}
	{

	}

	void LivesUIComponent::Update()
	{
		if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
	}

	void LivesUIComponent::PrepareImGuiRender()
	{
	}

	void LivesUIComponent::Notify(LivesComponent* pSubject)
	{
		m_pTextComponent->SetText("Remaining lives: " + std::to_string(pSubject->GetNrOfLives()));
	}
}
