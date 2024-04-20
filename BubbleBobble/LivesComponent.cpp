#include "LivesComponent.h"
#include "LivesUIComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

LivesComponent::LivesComponent(dae::GameObject* pOwner, int nrOfLives, LivesUIComponent* pObserver) :
	dae::Component{ pOwner },
	m_Lives{ nrOfLives },
	m_LivesChanged{ std::make_unique<dae::Subject<LivesComponent>>() }
{
	m_LivesChanged->AddObserver(pObserver);
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
	m_LivesChanged->NotifyObservers(this);
}

int LivesComponent::GetNrOfLives() const
{
	return m_Lives;
}
