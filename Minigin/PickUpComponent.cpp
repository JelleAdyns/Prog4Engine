#include "PickUpComponent.h"

namespace dae
{

	PickUpComponent::PickUpComponent(GameObject* pOwner, int scoreValue):
		Component{pOwner},
		Subject{},
		m_ScoreValue{scoreValue}
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
		NotifyObservers();
	}

	int PickUpComponent::GetScoreValue() const
	{
		return m_ScoreValue;
	}

}
