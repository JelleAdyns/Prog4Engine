#include "HitState.h"
#include "States.h"
#include <PhysicsComponent.h>
#include <Minigin.h>
#include <InputCommandBinder.h>

const float HitState::m_HitSpriteOffset{ 128.f };

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
	//m_pSpriteComp->SetStartRow(0);
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

	for (dae::Subject<SpriteComponent>* pSpriteSubject : m_pVecObservedSpriteSubjects)
	{
		pSpriteSubject->RemoveObserver(this);
	}

	m_pPlayer->GetComponent<dae::PhysicsComponent>()->StartGravity();

	m_pMovementComp->RegisterAttackCommand();
	m_pMovementComp->RegisterMoveCommands();

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


		m_pPlayerComp->Respawn();
	}
}

void HitState::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.emplace_back(pSubject);
}

float HitState::GetHitSpriteOffset()
{
	return m_HitSpriteOffset;
}

