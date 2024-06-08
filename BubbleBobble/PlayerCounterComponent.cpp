#include "PlayerCounterComponent.h"
#include "TimerComponent.h"
#include "LevelState.h"
#include <GameTime.h>
#include <AudioLocator.h>

PlayerCounterComponent::PlayerCounterComponent(dae::GameObject* pOwner) :
	Component{ pOwner },
	m_pTimerComp{},
	m_pVecObservedSubjects{}
{
}

PlayerCounterComponent::~PlayerCounterComponent()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		if (pSubject) pSubject->RemoveObserver(this);
	}
}

void PlayerCounterComponent::Start()
{
	if (!m_pTimerComp) m_pTimerComp = GetOwner()->GetComponent<TimerComponent>();
}

void PlayerCounterComponent::Update()
{

}

void PlayerCounterComponent::PrepareImGuiRender()
{
}

void PlayerCounterComponent::Notify(PlayerComponent*)
{
	--m_AmountOfPlayers;
	assert((m_AmountOfPlayers >= 0) && "Amount of enemies was lower then 0.");
	if (m_AmountOfPlayers == 0)
	{
		m_pTimerComp->Activate();
	}
}

void PlayerCounterComponent::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSubjects.emplace_back(pSubject);
	++m_AmountOfPlayers;
}

void PlayerCounterComponent::SetSubjectPointersInvalid(dae::Subject<PlayerComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pSubject);
	if (pos != m_pVecObservedSubjects.cend())
	{
		m_pVecObservedSubjects.erase(pos);
	}
}
