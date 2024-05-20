#include "PickUpComponent.h"
#include "ScoreUIComponent.h"

PickUpComponent::PickUpComponent(dae::GameObject* pOwner, PickUpComponent::PickUpType pickUpType, ScoreUIComponent* pObserver):
	dae::Component{pOwner},
	m_PickUpType{pickUpType},
	m_PickedUp{std::make_unique<dae::Subject<PickUpComponent>>()}
{
	m_PickedUp->AddObserver(pObserver);
}

void PickUpComponent::Start()
{
}

void PickUpComponent::Update()
{
}

void PickUpComponent::PrepareImGuiRender()
{
}

void PickUpComponent::PickUp()
{
	m_PickedUp->NotifyObservers(this);
}

PickUpComponent::PickUpType PickUpComponent::GetPickUpType() const
{
	return m_PickUpType;
}
