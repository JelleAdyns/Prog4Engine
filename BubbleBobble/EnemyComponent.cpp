#include "EnemyComponent.h"
#include "PlayerComponent.h"
#include <CollisionComponent.h>
#include <GameObject.h>

EnemyComponent::EnemyComponent(dae::GameObject* pOwner, EnemyType enemyType):
	dae::Component{pOwner},
	m_EnemyType{enemyType},
	m_pCurrState{},
	m_pPhysicsComp{},
	m_pCollisionComp{},
	m_pSpriteComp{},
	m_pCollided{std::make_unique<dae::Subject<EnemyComponent>>()},
	m_SubjectsForRunState{}
{
	//m_pCollided->AddObserver(pObserver);
}

void EnemyComponent::Update()
{
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();

	UpdateStates();

	if (m_pPhysicsComp->GetVelocity().x < 0) m_pSpriteComp->LookLeft(true);
	if (m_pPhysicsComp->GetVelocity().x > 0) m_pSpriteComp->LookLeft(false);

	if (m_pCollisionComp->GetCollisionFlags() > 0) m_pCollided->NotifyObservers(this);

}

void EnemyComponent::PrepareImGuiRender()
{
}

void EnemyComponent::AddPlayerObserver(PlayerComponent* pObserver)
{
	m_pCollided->AddObserver(pObserver);
	m_SubjectsForRunState.push_back(pObserver->GetSubject());
}

void EnemyComponent::UpdateStates()
{
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

	auto pNewState = m_pCurrState->Update();

	if (pNewState)
	{
		m_pCurrState->OnExit();
		m_pCurrState = std::move(pNewState);
		m_pCurrState->OnEnter();
	}
}
