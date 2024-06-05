#include "EnemyComponent.h"
#include "MaitaRunState.h"
#include "MaitaAttackState.h"
#include "ZenChanRunState.h"
#include "PlayerComponent.h"
#include "SpriteComponent.h"
#include "EnemyCounterComponent.h"
#include <PhysicsComponent.h>
#include <GameObject.h>

EnemyComponent::EnemyComponent(dae::GameObject* pOwner, EnemyType enemyType, EnemyCounterComponent* pObserver) :
	dae::Component{ pOwner },
	m_EnemyType{ enemyType },
	m_pCurrState{},
	m_pPhysicsComp{},
	m_pSpriteComp{},
	m_pDied{ std::make_unique<dae::Subject<EnemyComponent>>() },
	m_pVecObservedSubjects{}
{
	m_pDied->AddObserver(pObserver);
}

EnemyComponent::~EnemyComponent()
{
	m_pDied->NotifyObservers(this);

	for (auto& pSubject : m_pVecObservedSubjects)
	{
		if (pSubject) pSubject->RemoveObserver(this);
	}
}

void EnemyComponent::Start()
{
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();

	if (!m_pCurrState)
	{
		switch (m_EnemyType)
		{
		case EnemyType::ZenChan:
			m_pCurrState = std::make_unique<ZenChanRunState>(GetOwner(), this);
			break;
		case EnemyType::Maita:
			m_pCurrState = std::make_unique<MaitaRunState>(GetOwner(), this);
			break;
		}
		m_pCurrState->OnEnter();
	}
}

void EnemyComponent::Update()
{

	UpdateStates();

	if (m_pPhysicsComp->GetVelocity().x < 0) m_pSpriteComp->LookLeft(true);
	if (m_pPhysicsComp->GetVelocity().x > 0) m_pSpriteComp->LookLeft(false);

}

void EnemyComponent::PrepareImGuiRender()
{
}

void EnemyComponent::Notify(PlayerComponent* pSubject)
{
	m_pCurrState->NotifyPlayerObservers(pSubject);
}

void EnemyComponent::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSubjects.push_back(pSubject);
}

void EnemyComponent::SetSubjectPointersInvalid(dae::Subject<PlayerComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pSubject);
	if (pos != m_pVecObservedSubjects.cend())
	{
		m_pVecObservedSubjects.erase(pos);
	}
}

void EnemyComponent::Attack()
{
	m_pCurrState->OnExit();
	m_pCurrState = std::make_unique<MaitaAttackState>(GetOwner(), this);
	m_pCurrState->OnEnter();
}

void EnemyComponent::AddPlayerObserver(PlayerComponent* pSubject)
{
	if (pSubject) pSubject->GetSubject()->AddObserver(this);
}

void EnemyComponent::UpdateStates()
{
	auto pNewState = m_pCurrState->Update();

	if (pNewState)
	{
		m_pCurrState->OnExit();
		m_pCurrState = std::move(pNewState);
		m_pCurrState->OnEnter();
	}
}