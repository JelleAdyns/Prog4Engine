#include "TimerComponent.h"
#include <GameTime.h>

TimerComponent::TimerComponent(dae::GameObject* pOwner, float timeToExecute, std::unique_ptr<dae::Command>& pCommand):
	dae::Component{pOwner},
	m_IsActive{},
	m_TimeToExecute{timeToExecute},
	m_pCommandToExecute{std::move(pCommand)}
{
}

void TimerComponent::Start()
{
}

void TimerComponent::Update()
{
	if (m_IsActive)
	{
		m_Timer += dae::GameTime::GetInstance().GetDeltaTime();
		if (m_Timer >= m_TimeToExecute)
		{
			m_pCommandToExecute->Execute();
			m_IsActive = false;
		}
	}
}

void TimerComponent::PrepareImGuiRender()
{
}

void TimerComponent::Activate()
{
	m_IsActive = true;
}
