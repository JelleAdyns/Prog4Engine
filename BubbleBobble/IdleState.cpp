#include "IdleState.h"
#include "HitState.h"
#include "States.h"



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

	SpriteComponent* pSpriteComp = m_pPlayer->GetComponent<SpriteComponent>();
	pSpriteComp->SetHeightMarkers(0, HitState::GetHitSpriteOffset());
	pSpriteComp->SetNrOfRows(m_IdleSpriteInfo.nrOfRows);
	pSpriteComp->SetRowUpdate(m_IdleSpriteInfo.rowUpdate);

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

	SpriteComponent* pSpriteComp = m_pPlayer->GetComponent<SpriteComponent>();
	if (pSpriteComp->GetCurrRow() < GetShootStartIndex())
	{
		//m_IsShooting = true;
		pSpriteComp->SetFrameTime(0.2f);
		//pSpriteComp->SetStartRow(4);
		pSpriteComp->SetCol(0);
		pSpriteComp->SetRow(GetShootStartIndex() + m_IdleSpriteInfo.rowNumber);
	}
}

void IdleState::StopShooting()
{
	SpriteComponent* pSpriteComp = m_pPlayer->GetComponent<SpriteComponent>();
	pSpriteComp->SetNrOfRows(m_IdleSpriteInfo.nrOfRows);
	pSpriteComp->SetRow(m_IdleSpriteInfo.rowNumber);
	pSpriteComp->SetFrameTime(m_IdleSpriteInfo.frameTime);
}
