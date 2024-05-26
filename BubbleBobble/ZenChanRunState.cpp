#include "ZenChanRunState.h"
#include "ZenChanFallingState.h"
#include "ZenChanJumpState.h"
#include "ZenChanCaughtState.h"
#include "EnemyComponent.h"
#include "SpriteComponent.h"
#include "WallCheckingComponent.h"
#include "FloorCheckingComponent.h"
#include <GameTime.h>
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <CollisionComponent.h>

ZenChanRunState::ZenChanRunState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry) :
	ZenChanState{},
	m_Speed{ isAngry ? m_GeneralSpeed * 2.f : m_GeneralSpeed },
	m_IsAngry{isAngry},
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
	m_pWallCheckingComp{ pEnemy->GetComponent<WallCheckingComponent>() },
	m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() }
{}
std::unique_ptr<EnemyState> ZenChanRunState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if(!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<ZenChanCaughtState>(m_pEnemy, pCollidedObject);
		}
	}

	if (m_HasToJump) return std::make_unique<ZenChanJumpState>(m_pEnemy, m_pEnemyComp, m_IsAngry);
	
	if (!m_pFloorCheckingComp->IsOnGround()) return std::make_unique<ZenChanFallingState>(m_pEnemy, m_pEnemyComp, m_IsAngry);

	if (m_pWallCheckingComp->CollidingWithLeft()) m_pPhysicsComp->SetVelocityX(m_Speed);
	if (m_pWallCheckingComp->CollidingWithRight()) m_pPhysicsComp->SetVelocityX(-m_Speed);

	return nullptr;
}
void ZenChanRunState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pEnemyComp->GetPlayerSubjects())
	{
		pSubject->AddObserver(this);
	}
	if (m_pSpriteComp->IsLookingLeft()) m_pPhysicsComp->SetVelocityX(-m_Speed);
	else m_pPhysicsComp->SetVelocityX(m_Speed);

	if(m_IsAngry) m_pSpriteComp->SetRow(1);
	else m_pSpriteComp->SetRow(0);

	
}
void ZenChanRunState::OnExit()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pVecObservedSpriteSubjects)
	{
		pSubject->RemoveObserver(this);
	}
}

void ZenChanRunState::Notify(PlayerComponent* pSubject)
{
	auto subjectPos = pSubject->GetPos();
	auto enemyPos = m_pEnemy->GetWorldPosition();
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
