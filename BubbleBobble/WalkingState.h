#ifndef WALKINGSTATE_H
#define WALKINGSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"
#include "IdleState.h"
#include "HitState.h"
#include "Commands.h"
#include "Spawners.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <KeyState.h>
#include <GameObject.h>
#include <InputCommandBinder.h>
#include "JumpingState.h"

class WalkingState final : public PlayerState
{
public:
	explicit WalkingState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{pPlayerComp},
		m_pMovementComp{pMovementComp},
		m_pSpriteComp{ pPlayer->GetComponent<SpriteComponent>() },
		m_pPhysicsComp{ pPlayer->GetComponent<dae::PhysicsComponent>() },
		m_pCollisionComp{ pPlayer->GetComponent<dae::CollisionComponent>() }
	{}
	virtual ~WalkingState() = default;

	WalkingState(const WalkingState&) = delete;
	WalkingState(WalkingState&&) noexcept = delete;
	WalkingState& operator= (const WalkingState&) = delete;
	WalkingState& operator= (WalkingState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override
	{
		if(!m_pPlayerComp->IsInvincible())
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
		if (velocity.y > 0.f)
		{
			return std::make_unique<FallingState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
		}

		if (std::abs(velocity.x) == 0.f)
		{
			return std::make_unique<IdleState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
		}


		return nullptr;
	}
	virtual void OnEnter() override
	{
		m_pSpriteComp->AddRows(m_WalkingSpriteInfo.rowNumber);
		m_pSpriteComp->SetNrOfRows(m_WalkingSpriteInfo.nrOfRows);
		if (m_pSpriteComp->GetCurrRow() < GetShootStartIndex())
		{
			m_pSpriteComp->SetFrameTime(m_WalkingSpriteInfo.frameTime);
		}

		m_pMovementComp->RegisterJumpCommand();

	}
	virtual void OnExit() override
	{
		m_pSpriteComp->AddRows(-m_WalkingSpriteInfo.rowNumber);
		m_pMovementComp->UnRegisterJumpCommand();
	}
	virtual void Shoot() override
	{
		if(m_pSpriteComp->GetCurrRow() < GetShootStartIndex())
		{
			m_pSpriteComp->SetFrameTime(0.1f);
			m_pSpriteComp->SetCol(0);
			m_pSpriteComp->SetRow(GetShootStartIndex() + m_WalkingSpriteInfo.rowNumber);

			spawners::SpawnBubble(m_pPlayer->GetWorldPosition(), m_pPlayerComp->GetPlayerType(), m_pSpriteComp->IsLookingLeft());
		}
	}

	virtual void StopShooting() override
	{
		m_pSpriteComp->SetNrOfRows(m_WalkingSpriteInfo.nrOfRows);
		m_pSpriteComp->SetRow(m_WalkingSpriteInfo.rowNumber);
		m_pSpriteComp->SetFrameTime(m_WalkingSpriteInfo.frameTime);
	}
private:
	static constexpr SpriteComponent::SpriteInfo m_WalkingSpriteInfo{.rowNumber = 1, .nrOfRows = 8, .frameTime = 0.1f };
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	MovementComponent* m_pMovementComp;
	SpriteComponent* m_pSpriteComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
};


#endif // !WALKINGSTATE_H