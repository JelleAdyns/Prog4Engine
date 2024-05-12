#ifndef JUMPINGSTATE_H
#define JUMPINGSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "HitState.h"
#include "FallingState.h"
#include "Commands.h"
#include <KeyState.h>
#include <GameObject.h>
#include <InputCommandBinder.h>

class JumpingState final : public PlayerState
{
public:
	explicit JumpingState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp },
		m_pPhysicsComp{pPlayer->GetComponent<dae::PhysicsComponent>()}
	{}
	virtual ~JumpingState() = default;

	JumpingState(const JumpingState&) = delete;
	JumpingState(JumpingState&&) noexcept = delete;
	JumpingState& operator= (const JumpingState&) = delete;
	JumpingState& operator= (JumpingState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override
	{
		if (m_pPlayerComp->IsHit()) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp);

		if (m_pPhysicsComp->GetVelocity().y > 0.f)
		{
			return std::make_unique<FallingState>(m_pPlayer, m_pPlayerComp);
		}
		return nullptr;
	}
	virtual void OnEnter() const override
	{
		m_pPlayer->GetComponent<SpriteComponent>()->SetRow(2);

	}
	virtual void OnExit() const override
	{

	}
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	dae::PhysicsComponent* m_pPhysicsComp;
};


#endif // !JUMPINGSTATE_H