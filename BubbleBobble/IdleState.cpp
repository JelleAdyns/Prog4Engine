#include "IdleState.h"
#include "States.h"

std::unique_ptr<PlayerState> IdleState::Update() const
{

	auto velocity = m_pPhysicsComp->GetVelocity();

	if (velocity.y < 0.f)
	{
		return std::make_unique<JumpingState>(m_pPlayer, m_pPlayerComp);
	}

	if (std::abs(velocity.x) > 0.f)
	{
		return std::make_unique<WalkingState>(m_pPlayer, m_pPlayerComp);
	}

	return nullptr;
}
void IdleState::OnEnter() const
{
	SpriteComponent* spriteComp = m_pPlayer->GetComponent<SpriteComponent>();
	spriteComp->SetRow(0);
	spriteComp->SetNrOfRows(4);
	spriteComp->SetHeightMarkers(0, 64);

	m_pPhysicsComp->SetVelocityX(0);
	m_pPhysicsComp->SetVelocityY(0);

	auto& inputMan = dae::InputCommandBinder::GetInstance();


	auto pCommand = std::make_shared<JumpCommand>(m_pPlayer, m_pPlayerComp->GetJumpVelocity());
	inputMan.AddKeyCommand(pCommand, SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(pCommand, dae::ControllerButton::Y, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());


}
void IdleState::OnExit() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
	inputMan.RemoveControllerCommand(dae::ControllerButton::Y, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());
}