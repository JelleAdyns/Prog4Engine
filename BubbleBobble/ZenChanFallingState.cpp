#include "ZenChanFallingState.h"
#include "ZenChanRunState.h"
#include "SpriteComponent.h"
#include "ZenChanComponent.h"
#include "FloorCheckingComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>

ZenChanFallingState::ZenChanFallingState(dae::GameObject* pZenChan, ZenChanComponent* pZenChanComp, bool isAngry) :
	ZenChanState{},
	m_IsAngry{ isAngry },
	m_pZenChan{ pZenChan },
	m_pZenChanComp{ pZenChanComp },
	m_pPhysicsComp{ pZenChan->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pZenChan->GetComponent<dae::CollisionComponent>() },
	m_pFloorCheckingComp{ pZenChan->GetComponent<FloorCheckingComponent>() }
{}


std::unique_ptr<ZenChanState> ZenChanFallingState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<ZenChanCaughtState>(m_pZenChan, pCollidedObject);
		}
	}

	if (m_pZenChan->GetWorldPosition().y > dae::Minigin::GetWindowSize().y)
	{
		m_pZenChan->SetLocalPos(m_pZenChan->GetLocalPosition().x, -50);
	}
	if (m_pFloorCheckingComp->IsOnGround())
	{
		return std::make_unique<ZenChanRunState>(m_pZenChan, m_pZenChanComp, m_IsAngry);
	}

	return nullptr;
}
void ZenChanFallingState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pZenChanComp->GetPlayerSubjects())
	{
		pSubject->AddObserver(this);
	}
	m_pPhysicsComp->SetVelocityX(0);
}
void ZenChanFallingState::OnExit()
{

	SpriteComponent* pSpriteComp = m_pZenChan->GetComponent<SpriteComponent>();

	if (m_PlayerXPos < m_pZenChan->GetWorldPosition().x) pSpriteComp->LookLeft(true);
	else pSpriteComp->LookLeft(false);
	
}

void ZenChanFallingState::Notify(PlayerComponent* pSubject)
{
	auto subjectPos = pSubject->GetPos();
	m_PlayerXPos = subjectPos.x;
}

void ZenChanFallingState::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSubjects.push_back(pSubject);
}

void ZenChanFallingState::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		pSubject = nullptr;
	}
}
