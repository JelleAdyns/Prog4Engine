#include "MaitaRunState.h"
#include "MaitaCaughtState.h"
#include "MaitaJumpState.h"
#include "MaitaFallingState.h"
#include "MaitaAttackState.h"
#include "SpriteComponent.h"
#include "BubbleComponent.h"
#include "EnemyComponent.h"
#include "WallCheckingComponent.h"
#include "FloorCheckingComponent.h"
#include "CollisionTags.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <GameTime.h>

MaitaRunState::MaitaRunState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry) :
	MaitaState{},
	m_Speed{ isAngry ? m_GeneralSpeed * 1.8f : m_GeneralSpeed },
	m_IsAngry{ isAngry },
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
	m_pWallCheckingComp{ pEnemy->GetComponent<WallCheckingComponent>() },
	m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() }
{}

std::unique_ptr<EnemyState> MaitaRunState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<MaitaCaughtState>(m_pEnemy, pCollidedObject);
		}
	}


	
	if (m_AttackDelayTimer < m_AttackDelay)
	{
		m_AttackDelayTimer += dae::GameTime::GetInstance().GetDeltaTime();
	}
	else
	{
		if (m_HasToAttack) return std::make_unique<MaitaAttackState>(m_pEnemy, m_pEnemyComp, m_IsAngry);
	}


	if (m_HasToJump) return std::make_unique<MaitaJumpState>(m_pEnemy, m_pEnemyComp, m_IsAngry);

	if (!m_pFloorCheckingComp->IsOnGround()) return std::make_unique<MaitaFallingState>(m_pEnemy, m_pEnemyComp, m_IsAngry);

	if (m_pWallCheckingComp->CollidingWithLeft()) m_pPhysicsComp->SetVelocityX(m_Speed);
	if (m_pWallCheckingComp->CollidingWithRight()) m_pPhysicsComp->SetVelocityX(-m_Speed);

	return nullptr;
}

void MaitaRunState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pEnemyComp->GetPlayerSubjects())
	{
		if(pSubject) pSubject->AddObserver(this);
	}
	if (m_pSpriteComp->IsLookingLeft()) m_pPhysicsComp->SetVelocityX(-m_Speed);
	else m_pPhysicsComp->SetVelocityX(m_Speed);

	m_pSpriteComp->SetRow(m_RunInfo.rowNumber);
	if (m_IsAngry) m_pSpriteComp->AddRows(1);

	m_pSpriteComp->SetFrameTime(m_RunInfo.frameTime);
	m_pSpriteComp->SetNrOfCols(m_RunInfo.nrOfCols);
	m_pSpriteComp->SetRowUpdate(m_RunInfo.rowUpdate);
}

void MaitaRunState::OnExit()
{
}

void MaitaRunState::Notify(PlayerComponent* pSubject)
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

	float subjectMiddleY{ subjectPos.y + pSubject->GetDestRectSize().y / 2 };
	if (subjectMiddleY > enemyPos.y && subjectMiddleY < enemyPos.y + m_pSpriteComp->GetDestRectSize().y)
	{
		m_HasToAttack = true;
	}

}

void MaitaRunState::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.push_back(pSubject);
}

void MaitaRunState::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSpriteSubjects)
	{
		pSubject = nullptr;
	}
}
