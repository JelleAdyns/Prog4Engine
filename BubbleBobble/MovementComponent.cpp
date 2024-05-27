#include "MovementComponent.h"
#include "MovementCommands.h"
#include "ShootCommand.h"
#include <InputCommandBinder.h>
#include <Controller.h>
#include <KeyState.h>


uint8_t MovementComponent::m_NrOfPlayers{};

MovementComponent::MovementComponent(dae::GameObject* pOwner, float jumpVelocity, float moveSpeed):
	dae::Component{pOwner},
	m_JumpVelocity{jumpVelocity},
	m_MoveSpeed{moveSpeed}
{
	m_PlayerIndex = m_NrOfPlayers;
	++m_NrOfPlayers;
}

MovementComponent::~MovementComponent()
{
	--m_NrOfPlayers;
}

void MovementComponent::Start()
{

	m_pJumpCommand = std::make_shared<JumpCommand>(GetOwner(), m_JumpVelocity);
	m_MoveRightCommand = std::make_shared<MoveCommand>(GetOwner(), m_MoveSpeed);
	m_MoveLeftCommand = std::make_shared<MoveCommand>(GetOwner(), -m_MoveSpeed);
	m_StopMovingCommand = std::make_shared<StopMovingCommand>(GetOwner());
	m_AttackCommand = std::make_shared<ShootCommand>(GetOwner());

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

	inputMan.AddKeyCommand(m_pJumpCommand, SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(m_pJumpCommand, dae::ControllerButton::Y, dae::KeyState::Pressed, m_PlayerIndex);
}

void MovementComponent::UnRegisterJumpCommand() const
{

	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
	inputMan.RemoveControllerCommand(dae::ControllerButton::Y, dae::KeyState::Pressed, m_PlayerIndex);
}

void MovementComponent::RegisterMoveCommands() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.AddKeyCommand(m_MoveRightCommand, SDL_SCANCODE_D, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(m_MoveRightCommand, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_PlayerIndex);

	inputMan.AddKeyCommand(m_MoveLeftCommand, SDL_SCANCODE_A, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(m_MoveLeftCommand, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_PlayerIndex);

	inputMan.AddKeyCommand(m_StopMovingCommand, SDL_SCANCODE_D, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(m_StopMovingCommand, dae::ControllerButton::DpadRight, dae::KeyState::UpThisFrame, m_PlayerIndex);

	inputMan.AddKeyCommand(m_StopMovingCommand, SDL_SCANCODE_A, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(m_StopMovingCommand, dae::ControllerButton::DpadLeft, dae::KeyState::UpThisFrame, m_PlayerIndex);
}

void MovementComponent::UnRegisterMoveCommands() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(SDL_SCANCODE_A, dae::KeyState::Pressed);
	inputMan.RemoveKeyCommand(SDL_SCANCODE_A, dae::KeyState::UpThisFrame);
	inputMan.RemoveKeyCommand(SDL_SCANCODE_D, dae::KeyState::Pressed);
	inputMan.RemoveKeyCommand(SDL_SCANCODE_D, dae::KeyState::UpThisFrame);

	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_PlayerIndex);
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadLeft, dae::KeyState::UpThisFrame, m_PlayerIndex);
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_PlayerIndex);
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadRight, dae::KeyState::UpThisFrame, m_PlayerIndex);
}

void MovementComponent::RegisterAttackCommand() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.AddKeyCommand(m_AttackCommand, SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(m_AttackCommand, dae::ControllerButton::A, dae::KeyState::DownThisFrame, m_PlayerIndex);
}

void MovementComponent::UnRegisterAttackCommand() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	inputMan.RemoveControllerCommand(dae::ControllerButton::A, dae::KeyState::DownThisFrame, m_PlayerIndex);
}
