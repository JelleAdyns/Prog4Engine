#include "HitState.h"
#include "States.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <Minigin.h>
#include <InputCommandBinder.h>
#include <algorithm>

const float HitState::m_HitSpriteOffset{ 128.f };

HitState::HitState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp) :
	PlayerState{},
	m_pPlayer{ pPlayer },
	m_pPlayerComp{ pPlayerComp },
	m_pMovementComp{ pMovementComp },
	m_pSpriteComp{ pPlayer->GetComponent<SpriteComponent>() }
{}

std::unique_ptr<PlayerState> HitState::Update()
{
	if (m_RowCount == m_HitSpriteInfo.nrOfRows)
	{
		return std::make_unique<IdleState>(m_pPlayer, m_pPlayerComp, m_pMovementComp);
	}

	return nullptr;
}
void HitState::OnEnter()
{
	m_pSpriteComp->AddObserver(this);

	dae::PhysicsComponent* pPhysicsComp = m_pPlayer->GetComponent<dae::PhysicsComponent>();
	pPhysicsComp->StopGravity();
	pPhysicsComp->SetVelocityY(0);
	pPhysicsComp->SetVelocityX(0);

	float prevDestHeight{ static_cast<float>(m_pSpriteComp->GetDestRectSize().y) };

	m_pSpriteComp->SetNrOfRows(m_HitSpriteInfo.nrOfRows);
	m_pSpriteComp->SetHeightMarkers(m_HitSpriteOffset, static_cast<float>(m_pSpriteComp->GetTextureSize().y));
	m_pSpriteComp->SetRow(m_HitSpriteInfo.rowNumber);
	m_pSpriteComp->SetCol(0);
	m_pSpriteComp->SetFrameTime(m_HitSpriteInfo.frameTime);
	m_pSpriteComp->SetRowUpdate(true);

	float currDestHeight{ static_cast<float>(m_pSpriteComp->GetDestRectSize().y)};

	m_pPlayer->SetLocalPos(m_pPlayer->GetLocalPosition().x, m_pPlayer->GetLocalPosition().y - (currDestHeight - prevDestHeight));

	auto& inputMan = dae::InputCommandBinder::GetInstance();

	m_pMovementComp->UnRegisterAttackCommand();
	m_pMovementComp->UnRegisterMoveCommands();

	inputMan.VibrateController(40, m_pMovementComp->GetPlayerIndex());

	

}
void HitState::OnExit()
{

	m_pPlayer->GetComponent<dae::PhysicsComponent>()->StartGravity();
	m_pPlayerComp->SetInvincible();

	m_pMovementComp->RegisterAttackCommand();
	m_pMovementComp->RegisterMoveCommands();

	m_pPlayerComp->TakeLife();
	m_pPlayerComp->Respawn();
}

void HitState::Shoot()
{
}

void HitState::StopShooting()
{
}

void HitState::Notify(SpriteComponent*)
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.VibrateController(0, m_pMovementComp->GetPlayerIndex());

	++m_RowCount;
	if (m_RowCount == m_HitSpriteInfo.nrOfRows)
	{
		int nrOfRows{ 8 };

		m_pSpriteComp->SetHeightMarkers(0, m_HitSpriteOffset);
		m_pSpriteComp->SetNrOfRows(nrOfRows);
		m_pSpriteComp->SetRow(0);
		m_pSpriteComp->SetCol(0);
		m_pSpriteComp->SetRowUpdate(false);

		m_pPlayer->GetComponent<dae::RenderComponent>()->SetNeedToRender(false);

	}
}

void HitState::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.emplace_back(pSubject);
}

void HitState::SetSubjectPointersInvalid(dae::Subject<SpriteComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSpriteSubjects.begin(), m_pVecObservedSpriteSubjects.end(), pSubject);
	if (pos != m_pVecObservedSpriteSubjects.cend())
	{
		m_pVecObservedSpriteSubjects.erase(pos);
	}
}

float HitState::GetHitSpriteOffset()
{
	return m_HitSpriteOffset;
}

