#ifndef JUMPINGSTATE_H
#define JUMPINGSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"
#include "HitState.h"
#include "FallingState.h"
#include "Commands.h"
#include "Spawners.h"
#include <KeyState.h>
#include <GameObject.h>
#include <CollisionComponent.h>
#include <InputCommandBinder.h>

class JumpingState final : public PlayerState
{
public:
	explicit JumpingState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp },
		m_pMovementComp{ pMovementComp },
		m_pSpriteComp{ pPlayer->GetComponent<SpriteComponent>() },
		m_pPhysicsComp{pPlayer->GetComponent<dae::PhysicsComponent>()},
		m_pCollisionComp{ pPlayer->GetComponent<dae::CollisionComponent>() }
	{}
	virtual ~JumpingState() = default;

	JumpingState(const JumpingState&) = delete;
	JumpingState(JumpingState&&) noexcept = delete;
	JumpingState& operator= (const JumpingState&) = delete;
	JumpingState& operator= (JumpingState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override
	{
		if (!m_pPlayerComp->IsInvincible())
		{
			m_pCollisionComp->CheckForCollision(collisionTags::enemyTag);
			if (m_pCollisionComp->GetCollisionFlags() > 0) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);

			m_pCollisionComp->CheckForCollision(collisionTags::projectileTag);
			if (m_pCollisionComp->GetCollisionFlags() > 0) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
		}

		if (m_pPhysicsComp->GetVelocity().y > 0.f)
		{
			return std::make_unique<FallingState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
		}
		return nullptr;
	}
	virtual void OnEnter() override
	{
		m_pPlayer->GetComponent<SpriteComponent>()->AddRows(m_JumpingSpriteInfo.rowNumber);

	}
	virtual void OnExit() override
	{
		m_pPlayer->GetComponent<SpriteComponent>()->AddRows(-m_JumpingSpriteInfo.rowNumber);

	}
	virtual void Shoot() override
	{

		if (m_pSpriteComp->GetCurrRow() < GetShootStartIndex())
		{
			m_pSpriteComp->SetFrameTime(0.1f);
			m_pSpriteComp->SetCol(0);
			m_pSpriteComp->SetRow(GetShootStartIndex() + m_JumpingSpriteInfo.rowNumber);

			spawners::SpawnBubble(m_pPlayer->GetWorldPosition(), m_pSpriteComp->IsLookingLeft());
		}
	}
	virtual void StopShooting() override
	{
		m_pSpriteComp->SetNrOfRows(m_JumpingSpriteInfo.nrOfRows);
		m_pSpriteComp->SetRow(m_JumpingSpriteInfo.rowNumber);
		m_pSpriteComp->SetFrameTime(m_JumpingSpriteInfo.frameTime);
	}
private:
	static constexpr SpriteComponent::SpriteInfo m_JumpingSpriteInfo{ .rowNumber = 2, .nrOfRows = 8, .frameTime{0.1f} };
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	MovementComponent* m_pMovementComp;
	SpriteComponent* m_pSpriteComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
};


#endif // !JUMPINGSTATE_H