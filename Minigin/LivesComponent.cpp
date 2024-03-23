#include "LivesComponent.h"
#include "LivesUIComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
namespace dae
{
	LivesComponent::LivesComponent(GameObject* pOwner, int nrOfLives) :
		Component{ pOwner },
		m_Lives{ nrOfLives }
		//m_pDieEvent{ std::make_unique<Subject<LivesComponent>>() }
	{

	}

	void LivesComponent::Update()
	{
	}

	void LivesComponent::PrepareImGuiRender()
	{
	}

	void LivesComponent::TakeLife()
	{
		--m_Lives;
		NotifyObservers();
	}

	int LivesComponent::GetNrOfLives() const
	{
		return m_Lives;
	}

	/*const std::unique_ptr<Subject<LivesComponent>>& LivesComponent::GetSubject()
	{
		return m_pDieEvent;
	}*/

}