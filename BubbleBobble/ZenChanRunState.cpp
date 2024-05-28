#include "ZenChanRunState.h"
#include "ZenChanFallingState.h"
#include "ZenChanJumpState.h"
#include "ZenChanCaughtState.h"
#include "ZenChanComponent.h"
#include "SpriteComponent.h"
#include "WallCheckingComponent.h"
#include "FloorCheckingComponent.h"
#include <GameTime.h>
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <CollisionComponent.h>

ZenChanRunState::ZenChanRunState(dae::GameObject* pZenChan, ZenChanComponent* pZenChanComp, bool isAngry) :
	ZenChanState{},
	m_Speed{ isAngry ? m_GeneralSpeed * 2.f : m_GeneralSpeed },
	m_IsAngry{ isAngry },
	m_pZenChan{ pZenChan },
	m_pZenChanComp{ pZenChanComp },
	m_pPhysicsComp{ pZenChan->GetComponent<dae::PhysicsComponent>() },
	m_pSpriteComp{ pZenChan->GetComponent<SpriteComponent>() },
	m_pWallCheckingComp{ pZenChan->GetComponent<WallCheckingComponent>() },
	m_pFloorCheckingComp{ pZenChan->GetComponent<FloorCheckingComponent>() },
	m_pCollisionComp{ pZenChan->GetComponent<dae::CollisionComponent>() }
{}
std::unique_ptr<ZenChanState> ZenChanRunState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<ZenChanCaughtState>(m_pZenChan, pCollidedObject);
		}
	}

	if (m_HasToJump) return std::make_unique<ZenChanJumpState>(m_pZenChan, m_pZenChanComp, m_IsAngry);

	if (!m_pFloorCheckingComp->IsOnGround()) return std::make_unique<ZenChanFallingState>(m_pZenChan, m_pZenChanComp, m_IsAngry);

	if (m_pWallCheckingComp->CollidingWithLeft()) m_pPhysicsComp->SetVelocityX(m_Speed);
	if (m_pWallCheckingComp->CollidingWithRight()) m_pPhysicsComp->SetVelocityX(-m_Speed);

	return nullptr;
}
void ZenChanRunState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pZenChanComp->GetPlayerSubjects())
	{
		pSubject->AddObserver(this);
	}
	if (m_pSpriteComp->IsLookingLeft()) m_pPhysicsComp->SetVelocityX(-m_Speed);
	else m_pPhysicsComp->SetVelocityX(m_Speed);

	if (m_IsAngry) m_pSpriteComp->SetRow(1);
	else m_pSpriteComp->SetRow(0);


}
void ZenChanRunState::OnExit()
{

}

void ZenChanRunState::Notify(PlayerComponent* pSubject)
{
	auto subjectPos = pSubject->GetPos();
	auto enemyPos = m_pZenChan->GetWorldPosition();
	if (subjectPos.y < enemyPos.y)
	{
		float subjectMiddleX{ subjectPos.x + pSubject->GetDestRectSize().x / 2 };
		if (subjectMiddleX > enemyPos.x && subjectMiddleX < enemyPos.x + m_pSpriteComp->GetDestRectSize().x)
		{
			m_HasToJump = true;
		}
	}
	
}

void ZenChanRunState::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.push_back(pSubject);
}

void ZenChanRunState::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSpriteSubjects)
	{
		pSubject = nullptr;
	}
}
