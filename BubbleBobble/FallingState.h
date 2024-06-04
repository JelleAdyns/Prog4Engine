#ifndef FALLINGSTATE_H
#define FALLINGSTATE_H

#include "PlayerState.h"
#include "IdleState.h"
#include "HitState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"
#include "Commands.h"
#include "Spawners.h"
#include <KeyState.h>
#include <GameObject.h>
#include <Minigin.h>
#include <InputCommandBinder.h>

class FallingState final : public PlayerState
{
public:
	explicit FallingState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp },
		m_pMovementComp{ pMovementComp },
		m_pFloorCheckingComp{pPlayer->GetComponent<FloorCheckingComponent>()},
		m_pSpriteComp{pPlayer->GetComponent<SpriteComponent>()},
		m_pCollisionComp{ pPlayer->GetComponent<dae::CollisionComponent>() }
	{}
	virtual ~FallingState() = default;

	FallingState(const FallingState&) = delete;
	FallingState(FallingState&&) noexcept = delete;
	FallingState& operator= (const FallingState&) = delete;
	FallingState& operator= (FallingState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override
	{

		if (!m_pPlayerComp->IsInvincible())
		{
			 m_pCollisionComp->CheckForCollision(collisionTags::enemyTag);
			if (m_pCollisionComp->GetCollisionFlags() > 0) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);

			m_pCollisionComp->CheckForCollision(collisionTags::projectileTag);
			if (m_pCollisionComp->GetCollisionFlags() > 0) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
		}
		 
		if (m_pPlayer->GetWorldPosition().y > dae::Minigin::GetWindowSize().y) m_pPlayer->SetLocalPos(m_pPlayer->GetLocalPosition().x, -50);

		if (m_pFloorCheckingComp->IsOnGround())
		{
			return std::make_unique<IdleState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
		}

		return nullptr;
	}
	virtual void OnEnter() override
	{
		m_pSpriteComp->AddRows(m_FallingSpriteInfo.rowNumber);
		m_pSpriteComp->SetNrOfRows(m_FallingSpriteInfo.nrOfRows);
		m_pSpriteComp->SetFrameTime(m_FallingSpriteInfo.frameTime);

		m_pMovementComp->RegisterJumpOnBubbleCommand();
	}
	virtual void OnExit() override
	{

		m_pMovementComp->UnRegisterJumpOnBubbleCommand();
		m_pSpriteComp->AddRows(-m_FallingSpriteInfo.rowNumber);
	}

	virtual void Shoot() override
	{
		if (m_pSpriteComp->GetCurrRow() < GetShootStartIndex())
		{
			m_pSpriteComp->SetFrameTime(0.1f);
			m_pSpriteComp->SetCol(0);
			m_pSpriteComp->SetRow(GetShootStartIndex() + m_FallingSpriteInfo.rowNumber);

			spawners::SpawnBubble(m_pPlayer->GetWorldPosition(), m_pPlayerComp->GetPlayerType(), m_pSpriteComp->IsLookingLeft());
		}
	}
	virtual void StopShooting() override
	{
		m_pSpriteComp->SetNrOfRows(m_FallingSpriteInfo.nrOfRows);
		m_pSpriteComp->SetRow(m_FallingSpriteInfo.rowNumber);
		m_pSpriteComp->SetFrameTime(m_FallingSpriteInfo.frameTime);
	}
private:
	static constexpr SpriteComponent::SpriteInfo m_FallingSpriteInfo{ .rowNumber = 3, .nrOfRows = 8, .frameTime{0.1f} };

	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	MovementComponent* m_pMovementComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
	SpriteComponent* m_pSpriteComp;
	dae::CollisionComponent* m_pCollisionComp;
};


#endif // !FALLINGSTATE_H