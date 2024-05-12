#ifndef HITSTATE_H
#define HITSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "Commands.h"
#include <KeyState.h>
#include <GameObject.h>
#include <InputCommandBinder.h>

class HitState final : public PlayerState
{
public:
	explicit HitState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp }
	{}
	virtual ~HitState() = default;

	HitState(const HitState&) = delete;
	HitState(HitState&&) noexcept = delete;
	HitState& operator= (const HitState&) = delete;
	HitState& operator= (HitState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() const override
	{
		return nullptr;
	}
	virtual void OnEnter() const override
	{
		SpriteComponent* pSpriteComp = m_pPlayer->GetComponent<SpriteComponent>();
		pSpriteComp->SetHeightMarkers(128, static_cast<float>(pSpriteComp->GetTextureSize().y));
		pSpriteComp->SetNrOfRows(3);
		pSpriteComp->SetRow(0);
		pSpriteComp->SetCol(0);
		pSpriteComp->SetRowUpdate(true);


		auto& inputMan = dae::InputCommandBinder::GetInstance();

		inputMan.RemoveKeyCommand(SDL_SCANCODE_A, dae::KeyState::Pressed);
		inputMan.RemoveKeyCommand(SDL_SCANCODE_D, dae::KeyState::Pressed);
		inputMan.RemoveControllerCommand(dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());
		inputMan.RemoveControllerCommand(dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());

	}
	virtual void OnExit() const override
	{

		SpriteComponent* pSpriteComp = m_pPlayer->GetComponent<SpriteComponent>();
		pSpriteComp->SetHeightMarkers(128, static_cast<float>(pSpriteComp->GetTextureSize().y));
		pSpriteComp->SetNrOfRows(3);
		pSpriteComp->SetRow(0);
		pSpriteComp->SetCol(0);
		pSpriteComp->SetRowUpdate(true);

		auto& inputMan = dae::InputCommandBinder::GetInstance();

		std::shared_ptr<dae::Command> moveCommand = std::make_shared<MoveCommand>(m_pPlayer, m_pPlayerComp->GetMoveVelocity());
		inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_D, dae::KeyState::Pressed);
		inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());

		moveCommand = std::make_shared<MoveCommand>(m_pPlayer, -m_pPlayerComp->GetMoveVelocity());
		inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_A, dae::KeyState::Pressed);
		inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());
	}
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;

};


#endif // !HITSTATE_H