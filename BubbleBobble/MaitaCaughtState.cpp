#include "MaitaCaughtState.h"
#include "MaitaRunState.h"
#include "MaitaPoppedState.h"
#include "MaitaComponent.h"
#include "FloorCheckingComponent.h"
#include "BubbleComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>

MaitaCaughtState::MaitaCaughtState(dae::GameObject* pMaita, dae::GameObject* pBubble) :
	MaitaState{},
	m_pMaita{ pMaita },
	m_pPhysicsComp{ pMaita->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pMaita->GetComponent<dae::CollisionComponent>() },
	m_pSpriteComp{ pMaita->GetComponent<SpriteComponent>() },
	m_pOccupiedBubble{ pBubble->GetComponent<BubbleComponent>() },
	m_pObservedSubject{ nullptr }
{
	assert(pBubble->HasComponent<BubbleComponent>() && "The passed GameObject didn't have a BubbleComponent. Not good!");
	m_pMaita->SetParent(pBubble, false);

	dae::RenderComponent* pBubbleRenderComp = pBubble->GetComponent<dae::RenderComponent>();
	pBubbleRenderComp->SetNeedToRender(false);
};


std::unique_ptr<MaitaState> MaitaCaughtState::Update()
{
	switch (m_NextState)
	{
	case MaitaCaughtState::NextState::Popped:
		return std::make_unique<MaitaPoppedState>(m_pMaita);
		break;
	case MaitaCaughtState::NextState::Free:
		return std::make_unique<MaitaRunState>(m_pMaita, m_pMaita->GetComponent<MaitaComponent>(), true);
		break;
	}

	return nullptr;
}
void MaitaCaughtState::OnEnter()
{
	m_pOccupiedBubble->AddObserver(this);
	m_pOccupiedBubble->SetOccupied();


	switch (m_pOccupiedBubble->GetFloatingStage())
	{
	case BubbleComponent::FloatingStage::ShooterColor:
		m_pSpriteComp->SetRow(m_CaughtInfo.rowNumber);
		break;
	case BubbleComponent::FloatingStage::Pink:
		m_pSpriteComp->SetRow(m_CaughtInfo.rowNumber + 2);
		break;
	case BubbleComponent::FloatingStage::Red:
		m_pSpriteComp->SetRow(m_CaughtInfo.rowNumber + 3);
		break;
	}

	m_pSpriteComp->SetNrOfCols(m_CaughtInfo.nrOfCols, false);
	m_pMaita->GetComponent<FloorCheckingComponent>()->SetHandleCollison(false);
	m_pCollisionComp->SetTag(collisionTags::caughtEnemyTag);
	m_pMaita->SetLocalPos(-MaitaComponent::GetMaitaOffset(), 0.f);
	m_pPhysicsComp->SetVelocityX(0);
	m_pPhysicsComp->SetVelocityY(0);
	m_pPhysicsComp->StopGravity();

}
void MaitaCaughtState::OnExit()
{
	m_pMaita->GetComponent<FloorCheckingComponent>()->SetHandleCollison(true);
	m_pCollisionComp->SetTag(collisionTags::enemyTag);

	m_pPhysicsComp->StartGravity();
}

void MaitaCaughtState::Notify(BubbleComponent* pSubject)
{
	switch (pSubject->GetFloatingStage())
	{
	case BubbleComponent::FloatingStage::ShooterColor:
		m_pSpriteComp->SetRow(m_CaughtInfo.rowNumber + 2);
		break;
	case BubbleComponent::FloatingStage::Pink:
		m_pSpriteComp->AddRows(1);
		break;
	case BubbleComponent::FloatingStage::Red:
		m_pMaita->SetParent(nullptr, true);
		if (pSubject->IsPoppedByPlayer()) m_NextState = NextState::Popped;
		else m_NextState = NextState::Free;
		break;
	}
}
void MaitaCaughtState::AddSubjectPointer(dae::Subject<BubbleComponent>* pSubject)
{
	m_pObservedSubject = pSubject;
}

void MaitaCaughtState::SetSubjectPointersInvalid()
{
	m_pObservedSubject = nullptr;
}
