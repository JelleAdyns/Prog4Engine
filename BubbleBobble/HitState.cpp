#include "HitState.h"
#include "States.h"
#include <PhysicsComponent.h>
#include <Minigin.h>

const float HitState::m_HitSpriteOffset{ 128.f };

std::unique_ptr<PlayerState> HitState::Update()
{
;

	if (m_pPlayerComp->HitAnimFinished())
	{
		return std::make_unique<IdleState>(m_pPlayer, m_pPlayerComp);
	}

	if (!m_pPlayerComp->IsHit()) return std::make_unique<IdleState>(m_pPlayer, m_pPlayerComp);
	return nullptr;
}
void HitState::OnEnter() const
{


	m_pPlayer->GetComponent<dae::PhysicsComponent>()->StopGravity();
	

	float prevDestHeight{ static_cast<float>(m_pSpriteComp->GetDestRectSize().y) };

	m_pSpriteComp->SetNrOfRows(m_NrOfRows);
	m_pSpriteComp->SetHeightMarkers(m_HitSpriteOffset, static_cast<float>(m_pSpriteComp->GetTextureSize().y));
	m_pSpriteComp->SetStartRow(0);
	m_pSpriteComp->SetRow(0);
	m_pSpriteComp->SetCol(0);
	m_pSpriteComp->SetFrameTime(0.2f);
	m_pSpriteComp->SetRowUpdate(true);

	float currDestHeight{ static_cast<float>(m_pSpriteComp->GetDestRectSize().y)};

	m_pPlayer->SetLocalPos(m_pPlayer->GetLocalPosition().x, m_pPlayer->GetLocalPosition().y - (currDestHeight - prevDestHeight));

	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	inputMan.RemoveKeyCommand(SDL_SCANCODE_A, dae::KeyState::Pressed);
	inputMan.RemoveKeyCommand(SDL_SCANCODE_A, dae::KeyState::UpThisFrame);
	inputMan.RemoveKeyCommand(SDL_SCANCODE_D, dae::KeyState::Pressed);
	inputMan.RemoveKeyCommand(SDL_SCANCODE_D, dae::KeyState::UpThisFrame);
	inputMan.RemoveControllerCommand(dae::ControllerButton::A, dae::KeyState::DownThisFrame, m_pPlayerComp->GetPlayerIndex());
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadLeft, dae::KeyState::UpThisFrame, m_pPlayerComp->GetPlayerIndex());
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());
	inputMan.RemoveControllerCommand(dae::ControllerButton::DpadRight, dae::KeyState::UpThisFrame, m_pPlayerComp->GetPlayerIndex());



}
void HitState::OnExit() const
{
	int nrOfRows{ 4 };

	m_pPlayer->GetComponent<dae::PhysicsComponent>()->StartGravity();


	m_pSpriteComp->SetHeightMarkers(0, IdleState::GetNormalSpriteEndheight());
	m_pSpriteComp->SetNrOfRows(nrOfRows);
	m_pSpriteComp->SetRow(0);
	m_pSpriteComp->SetCol(0);
	m_pSpriteComp->SetRowUpdate(false);

	
	m_pPlayerComp->Respawn();


	auto& inputMan = dae::InputCommandBinder::GetInstance();

	std::shared_ptr<dae::Command> shootCommand = std::make_shared<ShootCommand>(m_pPlayer);
	inputMan.AddKeyCommand(shootCommand, SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(shootCommand, dae::ControllerButton::A, dae::KeyState::DownThisFrame, m_pPlayerComp->GetPlayerIndex());

	std::shared_ptr<dae::Command> moveCommand = std::make_shared<MoveCommand>(m_pPlayer, m_pPlayerComp->GetMoveVelocity());
	inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_D, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());

	moveCommand = std::make_shared<MoveCommand>(m_pPlayer, -m_pPlayerComp->GetMoveVelocity());
	inputMan.AddKeyCommand(moveCommand, SDL_SCANCODE_A, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(moveCommand, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());


	std::shared_ptr<dae::Command> stopMovingCommand = std::make_shared<StopMovingCommand>(m_pPlayer);
	inputMan.AddKeyCommand(stopMovingCommand, SDL_SCANCODE_D, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(stopMovingCommand, dae::ControllerButton::DpadRight, dae::KeyState::UpThisFrame, m_pPlayerComp->GetPlayerIndex());

	stopMovingCommand = std::make_shared<StopMovingCommand>(m_pPlayer);
	inputMan.AddKeyCommand(stopMovingCommand, SDL_SCANCODE_A, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(stopMovingCommand, dae::ControllerButton::DpadLeft, dae::KeyState::UpThisFrame, m_pPlayerComp->GetPlayerIndex());
}