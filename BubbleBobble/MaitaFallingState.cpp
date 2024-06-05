#include "MaitaFallingState.h"
#include "MaitaCaughtState.h"
#include "MaitaRunState.h"
#include "SpriteComponent.h"
#include "BubbleComponent.h"
#include "FloorCheckingComponent.h"
#include "EnemyComponent.h"
#include "CollisionTags.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <algorithm>

MaitaFallingState::MaitaFallingState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry) :
	MaitaState{},
	m_IsAngry{ isAngry },
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() },
	m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() }
{}


std::unique_ptr<EnemyState> MaitaFallingState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<MaitaCaughtState>(m_pEnemy, pCollidedObject);
		}
	}

	if (m_pEnemy->GetWorldPosition().y > dae::Minigin::GetWindowSize().y)
	{
		m_pEnemy->SetLocalPos(m_pEnemy->GetLocalPosition().x, -50);
	}
	if (m_pFloorCheckingComp->IsOnGround())
	{
		return std::make_unique<MaitaRunState>(m_pEnemy, m_pEnemyComp, m_IsAngry);
	}

	return nullptr;
}
void MaitaFallingState::OnEnter()
{
	m_pPhysicsComp->SetVelocityX(0);
}
void MaitaFallingState::OnExit()
{

	SpriteComponent* pSpriteComp = m_pEnemy->GetComponent<SpriteComponent>();

	if (m_PlayerXPos < m_pEnemy->GetWorldPosition().x) pSpriteComp->LookLeft(true);
	else pSpriteComp->LookLeft(false);

}

void MaitaFallingState::NotifyPlayerObservers(PlayerComponent* pSubject)
{

	auto subjectPos = pSubject->GetPos();
	auto enemyPos = m_pEnemy->GetWorldPosition();

	if (std::abs(enemyPos.x - subjectPos.x) < std::abs(enemyPos.x - m_PlayerXPos))
		m_PlayerXPos = subjectPos.x;
}
