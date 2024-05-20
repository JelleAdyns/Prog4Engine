#include "IdleState.h"
#include "States.h"

const float IdleState::m_NormalSpritesEndHeight{ 64.f };

std::unique_ptr<PlayerState> IdleState::Update()
{

	if (m_pPlayerComp->IsHit()) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);

	auto velocity = m_pPhysicsComp->GetVelocity();

	if (velocity.y < 0.f)
	{
		return std::make_unique<JumpingState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
	}

	if (std::abs(velocity.x) > 0.f)
	{
		return std::make_unique<WalkingState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
	}

	return nullptr;
}
void IdleState::OnEnter()
{
	int nrOfRows{ 4 };


	SpriteComponent* pSpriteComp = m_pPlayer->GetComponent<SpriteComponent>();
	pSpriteComp->SetHeightMarkers(0, m_NormalSpritesEndHeight);
	pSpriteComp->SetNrOfRows(nrOfRows);
	pSpriteComp->SetRow(0);
	pSpriteComp->SetRowUpdate(false);

	m_pPhysicsComp->SetVelocityX(0);
	m_pPhysicsComp->SetVelocityY(0);

	m_pMovementComp->RegisterJumpCommand();
}
void IdleState::OnExit()
{
	m_pMovementComp->UnRegisterJumpCommand();
}

void IdleState::Shoot()
{
}

float IdleState::GetNormalSpriteEndheight()
{
	return m_NormalSpritesEndHeight;
}