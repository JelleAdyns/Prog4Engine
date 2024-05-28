#include "MaitaFallingState.h"
#include "MaitaCaughtState.h"
#include "MaitaRunState.h"
#include "SpriteComponent.h"
#include "BubbleComponent.h"
#include "FloorCheckingComponent.h"
#include "MaitaComponent.h"
#include "CollisionTags.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>

MaitaFallingState::MaitaFallingState(dae::GameObject* pMaita, MaitaComponent* pMaitaComp, bool isAngry) :
	MaitaState{},
	m_IsAngry{ isAngry },
	m_pMaita{ pMaita },
	m_pMaitaComp{ pMaitaComp },
	m_pPhysicsComp{ pMaita->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pMaita->GetComponent<dae::CollisionComponent>() },
	m_pFloorCheckingComp{ pMaita->GetComponent<FloorCheckingComponent>() }
{}


std::unique_ptr<MaitaState> MaitaFallingState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<MaitaCaughtState>(m_pMaita, pCollidedObject);
		}
	}

	if (m_pMaita->GetWorldPosition().y > dae::Minigin::GetWindowSize().y)
	{
		m_pMaita->SetLocalPos(m_pMaita->GetLocalPosition().x, -50);
	}
	if (m_pFloorCheckingComp->IsOnGround())
	{
		return std::make_unique<MaitaRunState>(m_pMaita, m_pMaitaComp, m_IsAngry);
	}

	return nullptr;
}
void MaitaFallingState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pMaitaComp->GetPlayerSubjects())
	{
		pSubject->AddObserver(this);
	}
	m_pPhysicsComp->SetVelocityX(0);
}
void MaitaFallingState::OnExit()
{

	SpriteComponent* pSpriteComp = m_pMaita->GetComponent<SpriteComponent>();

	if (m_PlayerXPos < m_pMaita->GetWorldPosition().x) pSpriteComp->LookLeft(true);
	else pSpriteComp->LookLeft(false);

}

void MaitaFallingState::Notify(PlayerComponent* pSubject)
{
	auto subjectPos = pSubject->GetPos();
	m_PlayerXPos = subjectPos.x;
}

void MaitaFallingState::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSubjects.push_back(pSubject);
}

void MaitaFallingState::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		pSubject = nullptr;
	}
}
