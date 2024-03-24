#include "PickUpComponent.h"

namespace dae
{

	PickUpComponent::PickUpComponent(GameObject* pOwner):
		Component{pOwner},
		Subject{},
		m_ScoreValue{}
	{

	}

	void PickUpComponent::Update()
	{
	}

	void PickUpComponent::PrepareImGuiRender()
	{
	}

	void PickUpComponent::PickUp(int scoreValue)
	{
		m_ScoreValue = scoreValue;
		NotifyObservers();
	}

	int PickUpComponent::GetScoreValue() const
	{
		return m_ScoreValue;
	}

}
