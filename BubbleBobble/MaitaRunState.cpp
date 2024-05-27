#include "MaitaRunState.h"
#include "SpriteComponent.h"
#include "MaitaComponent.h"
#include <PhysicsComponent.h>
#include <GameObject.h>

MaitaRunState::MaitaRunState(dae::GameObject* pEnemy, MaitaComponent* pEnemyComp, bool isAngry) :
	MaitaState{},
	m_Speed{ isAngry ? m_GeneralSpeed * 2.f : m_GeneralSpeed },
	m_IsAngry{ false },
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
	m_pWallCheckingComp{ pEnemy->GetComponent<WallCheckingComponent>() },
	m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() }
{}

std::unique_ptr<MaitaState> MaitaRunState::Update()
{
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
		pSubject->AddObserver(this);
	}
	if (m_pSpriteComp->IsLookingLeft()) m_pPhysicsComp->SetVelocityX(-m_Speed);
	else m_pPhysicsComp->SetVelocityX(m_Speed);

	if (m_IsAngry) m_pSpriteComp->SetRow(1);
	else m_pSpriteComp->SetRow(0);
}

void MaitaRunState::OnExit()
{
}

void MaitaRunState::Notify(PlayerComponent* pSubject)
{
	auto subjectPos = pSubject->GetPos();
	/*auto enemyPos = m_pEnemy->GetWorldPosition();
	if (subjectPos.y < enemyPos.y)
	{
		float subjectMiddleX{ subjectPos.x + pSubject->GetDestRectSize().x / 2 };
		if (subjectMiddleX > enemyPos.x && subjectMiddleX < enemyPos.x + m_pSpriteComp->GetDestRectSize().x)
		{
			m_HasToJump = true;
		}
	}*/

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
