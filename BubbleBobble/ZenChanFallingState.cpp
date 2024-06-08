#include "ZenChanFallingState.h"
#include "ZenChanRunState.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include "FloorCheckingComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <algorithm>
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
		return std::make_unique<ZenChanCaughtState>(m_pEnemy, pCollidedObject);
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
	m_pPhysicsComp->SetVelocityX(0);
}
void ZenChanFallingState::OnExit()
{

	SpriteComponent* pSpriteComp = m_pEnemy->GetComponent<SpriteComponent>();

	if (m_PlayerXPos < m_pEnemy->GetWorldPosition().x) pSpriteComp->LookLeft(true);
	else pSpriteComp->LookLeft(false);
	
}

void ZenChanFallingState::NotifyPlayerObservers(PlayerComponent* pSubject)
{

	auto subjectPos = pSubject->GetPos();
	auto enemyPos = m_pEnemy->GetWorldPosition();

	if (std::abs(enemyPos.x - subjectPos.x) < std::abs(enemyPos.x - m_PlayerXPos) || m_PlayerXPos < 1.f)
		m_PlayerXPos = subjectPos.x;
}
