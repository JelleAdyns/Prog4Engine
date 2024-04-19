#include "ButtonHandlerComponent.h"
#include <stdexcept>
#include <algorithm>

ButtonHandlerComponent::ButtonHandlerComponent(dae::GameObject* pOwner):
	Component{pOwner},
	m_ButtonIsActivated{false},
	m_SelectedIndex{0},
	m_pVecButtons{}
{
}

void ButtonHandlerComponent::Update()
{
	if (m_ButtonIsActivated)
	{
		m_pVecButtons.at(m_SelectedIndex)->Activate();
		m_ButtonIsActivated = false;
	}
}

void ButtonHandlerComponent::PrepareImGuiRender()
{
}

void ButtonHandlerComponent::Activate()
{
	m_ButtonIsActivated = true;
}

void ButtonHandlerComponent::AddButton(ButtonComponent* newButton)
{
	auto pos = std::find(m_pVecButtons.cbegin(), m_pVecButtons.cend(), newButton);
	if(pos != m_pVecButtons.cend()) throw std::runtime_error("Vector already has pointer to this button");

	m_pVecButtons.emplace_back(newButton);
	if(m_pVecButtons.size() == 1) m_pVecButtons.at(m_SelectedIndex)->Select(true);
}

void ButtonHandlerComponent::SelectNext()
{
	m_pVecButtons.at(m_SelectedIndex)->Select(false);

	++m_SelectedIndex;
	if (m_SelectedIndex >= m_pVecButtons.size()) m_SelectedIndex = 0;

	m_pVecButtons.at(m_SelectedIndex)->Select(true);
}

void ButtonHandlerComponent::SelectPrevious()
{

	m_pVecButtons.at(m_SelectedIndex)->Select(false);

	--m_SelectedIndex;
	if (m_SelectedIndex < 0) m_SelectedIndex = static_cast<int>( m_pVecButtons.size() - 1 );

	m_pVecButtons.at(m_SelectedIndex)->Select(true);
}

ButtonComponent* ButtonHandlerComponent::GetSelectedButton() const
{
	return m_pVecButtons.at(m_SelectedIndex);
}
