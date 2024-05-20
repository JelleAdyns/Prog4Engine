#ifndef WALKINGSTATE_H
#define WALKINGSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"
#include "IdleState.h"
#include "HitState.h"
#include "Commands.h"
#include <PhysicsComponent.h>
#include <KeyState.h>
#include <GameObject.h>
#include <InputCommandBinder.h>

class WalkingState final : public PlayerState
{
public:
	explicit WalkingState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{pPlayerComp},
		m_pMovementComp{pMovementComp},
		m_pPhysicsComp{ pPlayer->GetComponent<dae::PhysicsComponent>() }
	{}
	virtual ~WalkingState() = default;

	WalkingState(const WalkingState&) = delete;
	WalkingState(WalkingState&&) noexcept = delete;
	WalkingState& operator= (const WalkingState&) = delete;
	WalkingState& operator= (WalkingState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override
	{

		if (m_pPlayerComp->IsHit()) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);

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
		m_pPlayer->GetComponent<SpriteComponent>()->AddRow(1);

		m_pMovementComp->RegisterJumpCommand();

	}
	virtual void OnExit() override
	{
		m_pPlayer->GetComponent<SpriteComponent>()->AddRow(-1);
		m_pMovementComp->UnRegisterJumpCommand();
	}
	virtual void Shoot() override
	{

	}
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	MovementComponent* m_pMovementComp;
	dae::PhysicsComponent* m_pPhysicsComp;
};


#endif // !WALKINGSTATE_H