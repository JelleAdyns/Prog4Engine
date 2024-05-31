#include "MaitaComponent.h"
#include "MaitaRunState.h"
#include "PlayerComponent.h"
#include "SpriteComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>

MaitaComponent::MaitaComponent(dae::GameObject* pOwner) :
	dae::Component{ pOwner },
	m_pCurrState{},
	m_pPhysicsComp{},
	m_pSpriteComp{},
	m_SubjectsForState{}
{
}

void MaitaComponent::Start()
{
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();
}

void MaitaComponent::Update()
{

	UpdateStates();

	if (m_pPhysicsComp->GetVelocity().x < 0)
	{
		m_pSpriteComp->LookLeft(true);
		//m_pCollisionComp->SetAdditionalOffset({ m_MaitaOffset,0.f });
	}
	if (m_pPhysicsComp->GetVelocity().x > 0)
	{
		m_pSpriteComp->LookLeft(false);
		//m_pCollisionComp->SetAdditionalOffset({ });
	}

}

void MaitaComponent::PrepareImGuiRender()
{
}

void MaitaComponent::AddPlayerObserver(PlayerComponent* pSubject)
{
	if(pSubject) m_SubjectsForState.push_back(pSubject->GetSubject());
}

void MaitaComponent::UpdateStates()
{
	if (!m_pCurrState)
	{
		m_pCurrState = std::make_unique<MaitaRunState>(GetOwner(), this);
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