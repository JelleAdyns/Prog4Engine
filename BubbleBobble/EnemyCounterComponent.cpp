#include "EnemyCounterComponent.h"
#include "LevelState.h"
#include <GameTime.h>

EnemyCounterComponent::EnemyCounterComponent(dae::GameObject* pOwner, std::unique_ptr<dae::Command>& pNextLevelCommand):
	Component{pOwner},
	m_pNextLevelCommand{std::move(pNextLevelCommand)},
	m_pVecObservedSubjects{}
{
}

EnemyCounterComponent::~EnemyCounterComponent()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		if (pSubject) pSubject->RemoveObserver(this);
	}
}

void EnemyCounterComponent::Start()
{
}

void EnemyCounterComponent::Update()
{
	if (m_AmountOfEnemies == 0)
	{
		m_Timer += dae::GameTime::GetInstance().GetDeltaTime();
		if (m_Timer >= m_TimeForLevelSwitch)
		{
			m_pNextLevelCommand->Execute();
		}
	}
}

void EnemyCounterComponent::PrepareImGuiRender()
{
}

void EnemyCounterComponent::Notify(EnemyComponent*)
{
	--m_AmountOfEnemies;
	assert((m_AmountOfEnemies >= 0) && "Amount of enemies was lower then 0.");
}

void EnemyCounterComponent::AddSubjectPointer(dae::Subject<EnemyComponent>* pSubject)
{
	m_pVecObservedSubjects.emplace_back(pSubject);
	++m_AmountOfEnemies;
}

void EnemyCounterComponent::SetSubjectPointersInvalid(dae::Subject<EnemyComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pSubject);
	if (pos != m_pVecObservedSubjects.cend())
	{
		m_pVecObservedSubjects.erase(pos);
	}
}
