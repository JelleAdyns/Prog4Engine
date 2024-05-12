#ifndef WALKINGSTATE_H
#define WALKINGSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
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
	explicit WalkingState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{pPlayerComp},
		m_pPhysicsComp{ pPlayer->GetComponent<dae::PhysicsComponent>() }
	{}
	virtual ~WalkingState() = default;

	WalkingState(const WalkingState&) = delete;
	WalkingState(WalkingState&&) noexcept = delete;
	WalkingState& operator= (const WalkingState&) = delete;
	WalkingState& operator= (WalkingState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override
	{

		if (m_pPlayerComp->IsHit()) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp);

		auto velocity = m_pPhysicsComp->GetVelocity();

		if (velocity.y < 0.f)
		{
			return std::make_unique<JumpingState>(m_pPlayer, m_pPlayerComp);
		}
		if (velocity.y > 0.f)
		{
			return std::make_unique<FallingState>(m_pPlayer, m_pPlayerComp);
		}

		if (std::abs(velocity.x) == 0.f)
		{
			return std::make_unique<IdleState>(m_pPlayer, m_pPlayerComp);
		}


		return nullptr;
	}
	virtual void OnEnter() const override
	{
		m_pPlayer->GetComponent<SpriteComponent>()->SetRow(1);


		auto& inputMan = dae::InputCommandBinder::GetInstance();
		auto pCommand = std::make_shared<JumpCommand>(m_pPlayer, m_pPlayerComp->GetJumpVelocity());
		inputMan.AddKeyCommand(pCommand, SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
		inputMan.AddControllerCommand(pCommand, dae::ControllerButton::Y, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());
		

	}
	virtual void OnExit() const override
	{
		auto& inputMan = dae::InputCommandBinder::GetInstance();
		
		inputMan.RemoveKeyCommand(SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
		inputMan.RemoveControllerCommand(dae::ControllerButton::Y, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());
		
	}
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	dae::PhysicsComponent* m_pPhysicsComp;
};


#endif // !WALKINGSTATE_H