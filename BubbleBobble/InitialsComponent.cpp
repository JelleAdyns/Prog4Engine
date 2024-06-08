#include "InitialsComponent.h"
#include <TextComponent.h>
#include <GameObject.h>
#include <Observer.h>

InitialsComponent::InitialsComponent(dae::GameObject* pOwner, dae::Observer<InitialsComponent>* pObserver):
	dae::Component{pOwner},
	m_pNameConfirmed{std::make_unique<dae::Subject<InitialsComponent>>()}
{
	m_pNameConfirmed->AddObserver(pObserver);
}

void InitialsComponent::Start()
{
	if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();
	m_pTextComponent->SetText(m_Characters);
}

void InitialsComponent::Update()
{
	if (m_Notify)
	{
		m_pNameConfirmed->NotifyObservers(this);
		m_Notify = false;
	}
}

void InitialsComponent::PrepareImGuiRender()
{
}

void InitialsComponent::ConfirmCharacter()
{
	++m_ActiveCharacter;
	if (m_ActiveCharacter == m_MaxChars)
	{
		m_Notify = true;
	}
	else if (m_ActiveCharacter < m_MaxChars) m_Characters[m_ActiveCharacter] = '.';

	m_pTextComponent->SetText(m_Characters);
}

void InitialsComponent::AdvanceCharacter(bool proceedForward)
{
	if (m_ActiveCharacter < m_MaxChars)
	{
		if (proceedForward)
		{
			if (m_Characters[m_ActiveCharacter] == 'Z') m_Characters[m_ActiveCharacter] = '.';
			else if (m_Characters[m_ActiveCharacter] == '.') m_Characters[m_ActiveCharacter] = 'A';
			else ++m_Characters[m_ActiveCharacter];
		}
		else
		{
			if (m_Characters[m_ActiveCharacter] == 'A') m_Characters[m_ActiveCharacter] = '.';
			else if (m_Characters[m_ActiveCharacter] == '.') m_Characters[m_ActiveCharacter] = 'Z';
			else --m_Characters[m_ActiveCharacter];
		}

		m_pTextComponent->SetText(m_Characters);
	}
}
