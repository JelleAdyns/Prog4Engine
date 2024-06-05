#include "MovementComponent.h"
#include "MovementCommands.h"
#include "AttackCommand.h"
#include <InputCommandBinder.h>
#include <Controller.h>
#include <KeyState.h>


MovementComponent::MovementComponent(dae::GameObject* pOwner, float jumpVelocity, float moveSpeed, uint8_t playerIndex):
	dae::Component{pOwner},
	m_PlayerIndex{playerIndex},
	m_JumpVelocity{jumpVelocity},
	m_MoveSpeed{moveSpeed}
{

}

MovementComponent::~MovementComponent()
{

	//UnRegisterMoveCommands();
	//UnRegisterAttackCommand();
	//UnRegisterJumpCommand();
}

void MovementComponent::Start()
{

	m_pJumpCommand = std::make_shared<JumpCommand>(GetOwner(), m_JumpVelocity);
	m_pJumpOnBubbleCommand = std::make_shared<JumpCommand>(GetOwner(), m_JumpVelocity, true);
	m_MoveRightCommand = std::make_shared<MoveCommand>(GetOwner(), m_MoveSpeed);
	m_MoveLeftCommand = std::make_shared<MoveCommand>(GetOwner(), -m_MoveSpeed);
	m_StopMovingCommand = std::make_shared<StopMovingCommand>(GetOwner());
	m_AttackCommand = std::make_shared<AttackCommand>(GetOwner());

	RegisterMoveCommands();
	RegisterAttackCommand();
}

void MovementComponent::Update()
{
}

void MovementComponent::PrepareImGuiRender()
{
}

void MovementComponent::RegisterJumpCommand() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.AddKeyCommand(m_pJumpCommand, m_PlayerIndex == 0 ? SDL_SCANCODE_SPACE : SDL_SCANCODE_UP, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(m_pJumpCommand, dae::ControllerButton::Y, dae::KeyState::Pressed, m_PlayerIndex);
}

void MovementComponent::UnRegisterJumpCommand() const
{

	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(m_PlayerIndex == 0 ? SDL_SCANCODE_SPACE : SDL_SCANCODE_UP, dae::KeyState::Pressed);
	inputMan.RemoveControllerCommand(dae::ControllerButton::Y, dae::KeyState::Pressed, m_PlayerIndex);
}

void MovementComponent::RegisterJumpOnBubbleCommand() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.AddKeyCommand(m_pJumpOnBubbleCommand, m_PlayerIndex == 0 ? SDL_SCANCODE_SPACE : SDL_SCANCODE_UP, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(m_pJumpOnBubbleCommand, dae::ControllerButton::Y, dae::KeyState::Pressed, m_PlayerIndex);
}
void MovementComponent::UnRegisterJumpOnBubbleCommand() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(m_PlayerIndex == 0 ? SDL_SCANCODE_SPACE : SDL_SCANCODE_UP, dae::KeyState::Pressed);
	inputMan.RemoveControllerCommand(dae::ControllerButton::Y, dae::KeyState::Pressed, m_PlayerIndex);
}

void MovementComponent::RegisterMoveCommands() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.AddKeyCommand(m_MoveRightCommand, m_PlayerIndex == 0 ? SDL_SCANCODE_D : SDL_SCANCODE_RIGHT, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(m_MoveRightCommand, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_PlayerIndex);

	inputMan.AddKeyCommand(m_MoveLeftCommand, m_PlayerIndex == 0 ? SDL_SCANCODE_A : SDL_SCANCODE_LEFT, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(m_MoveLeftCommand, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_PlayerIndex);

	inputMan.AddKeyCommand(m_StopMovingCommand, m_PlayerIndex == 0 ? SDL_SCANCODE_D : SDL_SCANCODE_RIGHT, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(m_StopMovingCommand, dae::ControllerButton::DpadRight, dae::KeyState::UpThisFrame, m_PlayerIndex);

	inputMan.AddKeyCommand(m_StopMovingCommand, m_PlayerIndex == 0 ? SDL_SCANCODE_A : SDL_SCANCODE_LEFT, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(m_StopMovingCommand, dae::ControllerButton::DpadLeft, dae::KeyState::UpThisFrame, m_PlayerIndex);
}

void MovementComponent::UnRegisterMoveCommands() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(m_PlayerIndex == 0 ? SDL_SCANCODE_A : SDL_SCANCODE_LEFT, dae::KeyState::Pressed);
	inputMan.RemoveKeyCommand(m_PlayerIndex == 0 ? SDL_SCANCODE_A : SDL_SCANCODE_LEFT, dae::KeyState::UpThisFrame);
	inputMan.RemoveKeyCommand(m_PlayerIndex == 0 ? SDL_SCANCODE_D : SDL_SCANCODE_RIGHT, dae::KeyState::Pressed);
	inputMan.RemoveKeyCommand(m_PlayerIndex == 0 ? SDL_SCANCODE_D : SDL_SCANCODE_RIGHT, dae::KeyState::UpThisFrame);

	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_PlayerIndex);
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadLeft, dae::KeyState::UpThisFrame, m_PlayerIndex);
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_PlayerIndex);
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadRight, dae::KeyState::UpThisFrame, m_PlayerIndex);
}

void MovementComponent::RegisterAttackCommand() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.AddKeyCommand(m_AttackCommand, m_PlayerIndex == 0 ? SDL_SCANCODE_W : SDL_SCANCODE_RCTRL, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(m_AttackCommand, dae::ControllerButton::A, dae::KeyState::DownThisFrame, m_PlayerIndex);
}

void MovementComponent::UnRegisterAttackCommand() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(m_PlayerIndex == 0 ? SDL_SCANCODE_W : SDL_SCANCODE_RCTRL, dae::KeyState::DownThisFrame);
	inputMan.RemoveControllerCommand(dae::ControllerButton::A, dae::KeyState::DownThisFrame, m_PlayerIndex);
}
