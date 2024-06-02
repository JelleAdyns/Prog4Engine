#include "MaitaJumpState.h"

MaitaJumpState::MaitaJumpState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry) :
	MaitaState{},
	m_IsAngry{ isAngry },
	m_TimeToJump{ isAngry ? 0.4f : 0.8f },
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() }
{}

std::unique_ptr<EnemyState> MaitaJumpState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<MaitaCaughtState>(m_pEnemy, pCollidedObject);
		}
	}

	auto deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
	m_TimeBeforeJump += deltaTime;
	m_TimeBeforeFlip += deltaTime;

	if (m_CheckIfLanded)
	{
		if (m_pPhysicsComp->GetVelocity().y > 0.f) return std::make_unique<MaitaFallingState>(m_pEnemy, m_pEnemyComp, m_IsAngry);
	}
	else
	{
		if (m_TimeBeforeFlip >= m_TimeToJump / 2.f)
		{
			m_pSpriteComp->Flip();
			m_TimeBeforeFlip = 0.f;
		}

		if (m_TimeBeforeJump >= m_TimeToJump)
		{
			m_pPhysicsComp->SetVelocityY(m_JumpVelocity);
			m_CheckIfLanded = true;
		}
	}

	return nullptr;
}
void MaitaJumpState::OnEnter()
{

	m_pSpriteComp->SetCol(0);
	m_pSpriteComp->SetUpdate(false);

	m_pSpriteComp->Flip();

	m_pPhysicsComp->SetVelocityX(0);
}
void MaitaJumpState::OnExit()
{

	m_pSpriteComp->SetUpdate(true);

}