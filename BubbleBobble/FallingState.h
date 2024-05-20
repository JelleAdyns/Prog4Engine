#ifndef FALLINGSTATE_H
#define FALLINGSTATE_H

#include "PlayerState.h"
#include "IdleState.h"
#include "HitState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"
#include "Commands.h"
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
		m_pFloorCheckingComp{pPlayer->GetComponent<FloorCheckingComponent>()}
	{}
	virtual ~FallingState() = default;

	FallingState(const FallingState&) = delete;
	FallingState(FallingState&&) noexcept = delete;
	FallingState& operator= (const FallingState&) = delete;
	FallingState& operator= (FallingState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override
	{
		if (m_pPlayerComp->IsHit()) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
		 
		if (m_pPlayer->GetWorldPosition().y > dae::Minigin::GetWindowSize().y) m_pPlayer->SetLocalPos(m_pPlayer->GetLocalPosition().x, -50);

		if (m_pFloorCheckingComp->IsOnGround())
		{
			return std::make_unique<IdleState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
		}

		return nullptr;
	}
	virtual void OnEnter() override
	{
		m_pPlayer->GetComponent<SpriteComponent>()->AddRow(3);

	}
	virtual void OnExit() override
	{

		m_pPlayer->GetComponent<SpriteComponent>()->AddRow(-3);
	}

	virtual void Shoot() override
	{

	}
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	MovementComponent* m_pMovementComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
};


#endif // !FALLINGSTATE_H