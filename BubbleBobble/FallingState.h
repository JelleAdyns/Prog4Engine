#ifndef FALINGSTATE_H
#define FALINGSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "Commands.h"
#include <KeyState.h>
#include <GameObject.h>
#include <InputCommandBinder.h>

class FallingState final : public PlayerState
{
public:
	explicit FallingState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp },
		m_pFloorCheckingComp{pPlayer->GetComponent<FloorCheckingComponent>()}
	{}
	virtual ~FallingState() = default;

	FallingState(const FallingState&) = delete;
	FallingState(FallingState&&) noexcept = delete;
	FallingState& operator= (const FallingState&) = delete;
	FallingState& operator= (FallingState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() const override
	{
		if (m_pFloorCheckingComp->IsOnGround())
		{
			return std::make_unique<IdleState>(m_pPlayer, m_pPlayerComp);
		}

		return nullptr;
	}
	virtual void OnEnter() const override
	{
		m_pPlayer->GetComponent<SpriteComponent>()->SetRow(3);

	}
	virtual void OnExit() const override
	{

	}
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
};


#endif // !FALINGSTATE_H