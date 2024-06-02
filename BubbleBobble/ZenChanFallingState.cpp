#include "ZenChanFallingState.h"
#include "ZenChanRunState.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include "FloorCheckingComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include "CollisionTags.h"
#include "BubbleComponent.h"
#include "ZenChanCaughtState.h"

ZenChanFallingState::ZenChanFallingState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry) :
	ZenChanState{},
	m_IsAngry{ isAngry },
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() },
	m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() }
{}


std::unique_ptr<EnemyState> ZenChanFallingState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<ZenChanCaughtState>(m_pEnemy, pCollidedObject);
		}
	}

	if (m_pEnemy->GetWorldPosition().y > dae::Minigin::GetWindowSize().y)
	{
		m_pEnemy->SetLocalPos(m_pEnemy->GetLocalPosition().x, -50);
	}
	if (m_pFloorCheckingComp->IsOnGround())
	{
		return std::make_unique<ZenChanRunState>(m_pEnemy, m_pEnemyComp, m_IsAngry);
	}

	return nullptr;
}
void ZenChanFallingState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pEnemyComp->GetPlayerSubjects())
	{
		pSubject->AddObserver(this);
	}
	m_pPhysicsComp->SetVelocityX(0);
}
void ZenChanFallingState::OnExit()
{

	SpriteComponent* pSpriteComp = m_pEnemy->GetComponent<SpriteComponent>();

	if (m_PlayerXPos < m_pEnemy->GetWorldPosition().x) pSpriteComp->LookLeft(true);
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
