#include "EnemyCounterComponent.h"
#include "LevelState.h"
#include <GameTime.h>
#include "TimerComponent.h"

EnemyCounterComponent::EnemyCounterComponent(dae::GameObject* pOwner):
	Component{pOwner},
	m_pTimerComp{},
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
	if (!m_pTimerComp) m_pTimerComp = GetOwner()->GetComponent<TimerComponent>();
}

void EnemyCounterComponent::Update()
{

}

void EnemyCounterComponent::PrepareImGuiRender()
{
}

void EnemyCounterComponent::Notify(EnemyComponent*)
{
	--m_AmountOfEnemies;
	assert((m_AmountOfEnemies >= 0) && "Amount of enemies was lower then 0.");
	if (m_AmountOfEnemies == 0)
	{
		m_pTimerComp->Activate();
	}
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
