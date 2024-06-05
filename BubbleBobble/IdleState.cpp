#include "IdleState.h"
#include "States.h"

#include "Spawners.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <CollisionComponent.h>

IdleState::IdleState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp) :
	PlayerState{},
	m_pPlayer{ pPlayer },
	m_pPlayerComp{ pPlayerComp },
	m_pMovementComp{ pMovementComp },
	m_pSpriteComp{ pPlayer->GetComponent<SpriteComponent>() },
	m_pPhysicsComp{ pPlayer->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pPlayer->GetComponent<dae::CollisionComponent>() }
{}

std::unique_ptr<PlayerState> IdleState::Update()
{

	if (!m_pPlayerComp->IsInvincible())
	{
		m_pCollisionComp->CheckForCollision(collisionTags::enemyTag);
		if (m_pCollisionComp->GetCollisionFlags() > 0) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);

		m_pCollisionComp->CheckForCollision(collisionTags::projectileTag);
		if (m_pCollisionComp->GetCollisionFlags() > 0) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
	}

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
	m_pSpriteComp->SetHeightMarkers(0, HitState::GetHitSpriteOffset());
	m_pSpriteComp->SetNrOfRows(m_IdleSpriteInfo.nrOfRows);
	m_pSpriteComp->SetRowUpdate(m_IdleSpriteInfo.rowUpdate);
	if (m_pSpriteComp->GetCurrRow() < GetShootStartIndex())
	{
		m_pSpriteComp->SetFrameTime(m_IdleSpriteInfo.frameTime);
	}
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

	if (m_pSpriteComp->GetCurrRow() < GetShootStartIndex())
	{
		m_pSpriteComp->SetFrameTime(0.1f);
		m_pSpriteComp->SetCol(0);
		m_pSpriteComp->SetRow(GetShootStartIndex() + m_IdleSpriteInfo.rowNumber);

		spawners::SpawnBubble(m_pPlayer->GetWorldPosition(), m_pPlayerComp->GetPlayerType(), m_pSpriteComp->IsLookingLeft());
	}
}

void IdleState::StopShooting()
{
	m_pSpriteComp->SetNrOfRows(m_IdleSpriteInfo.nrOfRows);
	m_pSpriteComp->SetRow(m_IdleSpriteInfo.rowNumber);
	m_pSpriteComp->SetFrameTime(m_IdleSpriteInfo.frameTime);
}
