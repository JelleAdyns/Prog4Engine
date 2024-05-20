#include "MovementComponent.h"
#include "JumpCommand.h"
#include "MovementCommands.h"
#include "ShootCommand.h"
#include <InputCommandBinder.h>
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
	auto pCommand = std::make_shared<JumpCommand>(GetOwner(), m_JumpVelocity);
	inputMan.AddKeyCommand(pCommand, SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(pCommand, dae::ControllerButton::Y, dae::KeyState::Pressed, m_PlayerIndex);
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

	std::shared_ptr<dae::Command> moveCommand = std::make_shared<MoveCommand>(GetOwner(), m_MoveSpeed);
	inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_D, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_PlayerIndex);

	moveCommand = std::make_shared<MoveCommand>(GetOwner(), -m_MoveSpeed);
	inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_A, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_PlayerIndex);

	std::shared_ptr<dae::Command> stopMovingCommand = std::make_shared<StopMovingCommand>(GetOwner());
	inputMan.AddKeyCommand(stopMovingCommand, SDL_SCANCODE_D, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(stopMovingCommand, dae::ControllerButton::DpadRight, dae::KeyState::UpThisFrame, m_PlayerIndex);

	stopMovingCommand = std::make_shared<StopMovingCommand>(GetOwner());
	inputMan.AddKeyCommand(stopMovingCommand, SDL_SCANCODE_A, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(stopMovingCommand, dae::ControllerButton::DpadLeft, dae::KeyState::UpThisFrame, m_PlayerIndex);
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

	std::shared_ptr<dae::Command> shootCommand = std::make_shared<ShootCommand>(GetOwner());
	inputMan.AddKeyCommand(shootCommand, SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(shootCommand, dae::ControllerButton::A, dae::KeyState::DownThisFrame, m_PlayerIndex);
}

void MovementComponent::UnRegisterAttackCommand() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	inputMan.RemoveControllerCommand(dae::ControllerButton::A, dae::KeyState::DownThisFrame, m_PlayerIndex);
}
