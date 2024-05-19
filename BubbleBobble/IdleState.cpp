#include "IdleState.h"
#include "States.h"

const float IdleState::m_NormalSpritesEndHeight{ 64.f };

std::unique_ptr<PlayerState> IdleState::Update()
{

	if (m_pPlayerComp->IsHit()) return std::make_unique<HitState>(m_pPlayer, m_pPlayerComp);

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
void IdleState::OnEnter()
{
	int nrOfRows{ 4 };


	SpriteComponent* pSpriteComp = m_pPlayer->GetComponent<SpriteComponent>();
	pSpriteComp->SetHeightMarkers(0, m_NormalSpritesEndHeight);
	pSpriteComp->SetNrOfRows(nrOfRows);
	pSpriteComp->SetRow(0);
	pSpriteComp->SetRowUpdate(false);

	m_pPhysicsComp->SetVelocityX(0);
	m_pPhysicsComp->SetVelocityY(0);

	auto& inputMan = dae::InputCommandBinder::GetInstance();


	auto pCommand = std::make_shared<JumpCommand>(m_pPlayer, m_pPlayerComp->GetJumpVelocity());
	inputMan.AddKeyCommand(pCommand, SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
	inputMan.AddControllerCommand(pCommand, dae::ControllerButton::Y, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());


}
void IdleState::OnExit()
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveKeyCommand(SDL_SCANCODE_SPACE, dae::KeyState::Pressed);
	inputMan.RemoveControllerCommand(dae::ControllerButton::Y, dae::KeyState::Pressed, m_pPlayerComp->GetPlayerIndex());
}

void IdleState::Shoot()
{
}

float IdleState::GetNormalSpriteEndheight()
{
	return m_NormalSpritesEndHeight;
}