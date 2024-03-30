#include "PickUpComponent.h"
#include "ScoreUIComponent.h"

namespace dae
{

	PickUpComponent::PickUpComponent(GameObject* pOwner, PickUpComponent::PickUpType pickUpType, ScoreUIComponent* pObserver):
		Component{pOwner},
		m_PickUpType{pickUpType},
		m_PickedUp{std::make_unique<Subject<PickUpComponent>>()}
	{
		m_PickedUp->AddObserver(pObserver);
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

}
