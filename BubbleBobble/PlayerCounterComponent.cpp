#include "PlayerCounterComponent.h"
#include "LevelState.h"
#include <GameTime.h>
#include <AudioLocator.h>

PlayerCounterComponent::PlayerCounterComponent(dae::GameObject* pOwner, std::unique_ptr<dae::Command>& pNextLevelCommand) :
	Component{ pOwner },
	m_pNextLevelCommand{ std::move(pNextLevelCommand) },
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
}

void PlayerCounterComponent::Update()
{
	if (m_AmountOfPlayers == 0)
	{
		m_Timer += dae::GameTime::GetInstance().GetDeltaTime();
		if (m_Timer >= m_TimeForLevelSwitch)
		{
			m_pNextLevelCommand->Execute();
		}
	}
}

void PlayerCounterComponent::PrepareImGuiRender()
{
}

void PlayerCounterComponent::Notify(PlayerComponent*)
{
	--m_AmountOfPlayers;
	assert((m_AmountOfPlayers >= 0) && "Amount of enemies was lower then 0.");
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
