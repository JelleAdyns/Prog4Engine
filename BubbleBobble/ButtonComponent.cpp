#include "ButtonComponent.h"
#include "GameObject.h"

ButtonComponent::ButtonComponent(dae::GameObject* pOwner, std::unique_ptr<dae::Command>& pCommand):
	dae::Component{pOwner},
	m_IsSelected{false},
	m_pCommandToExecute{ std::move(pCommand)}
{
}

void ButtonComponent::Update()
{
}

void ButtonComponent::PrepareImGuiRender()
{
}

void ButtonComponent::Activate() const
{
	if (m_IsSelected)
	{
		m_pCommandToExecute->Execute();
	}
}

void ButtonComponent::Select(bool select)
{
	m_IsSelected = select;
}

bool ButtonComponent::IsSelected() const
{
	return m_IsSelected;
}

glm::vec2 ButtonComponent::GetPos() const
{
	return GetOwner()->GetWorldPosition();
}
