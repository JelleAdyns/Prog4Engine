#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"

#include "Commands.h"
#include <PhysicsComponent.h>
#include <KeyState.h>
#include <GameObject.h>
#include <InputCommandBinder.h>

class IdleState final : public PlayerState
{
public:
	explicit IdleState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp },
		m_pMovementComp{ pMovementComp },
		m_pPhysicsComp{ pPlayer->GetComponent<dae::PhysicsComponent>() }
	{}
	virtual ~IdleState() = default;

	IdleState(const IdleState&) = delete;
	IdleState(IdleState&&) noexcept = delete;
	IdleState& operator= (const IdleState&) = delete;
	IdleState& operator= (IdleState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Shoot() override;
	virtual void StopShooting() override;

private:
	static constexpr SpriteComponent::RowInfo m_IdleSpriteInfo{ .rowNumber = 0, .nrOfRows = 8, .frameTime{0.1f} };
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	MovementComponent* m_pMovementComp;
	dae::PhysicsComponent* m_pPhysicsComp;

};


#endif // !IDLESTATE_H
