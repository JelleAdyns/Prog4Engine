#include "MaitaCaughtState.h"
#include "MaitaRunState.h"
#include "MaitaPoppedState.h"
#include "EnemyComponent.h"
#include "FloorCheckingComponent.h"
#include "BubbleComponent.h"
#include "CollisionTags.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>

MaitaCaughtState::MaitaCaughtState(dae::GameObject* pEnemy, dae::GameObject* pBubble) :
	MaitaState{},
	m_pEnemy{ pEnemy },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
	m_pOccupiedBubble{ pBubble->GetComponent<BubbleComponent>() },
	m_pObservedSubject{ nullptr }
{
	assert(pBubble->HasComponent<BubbleComponent>() && "The passed GameObject didn't have a BubbleComponent. Not good!");
	m_pEnemy->SetParent(pBubble, false);

	dae::RenderComponent* pBubbleRenderComp = pBubble->GetComponent<dae::RenderComponent>();
	pBubbleRenderComp->SetNeedToRender(false);
};


std::unique_ptr<EnemyState> MaitaCaughtState::Update()
{
	switch (m_NextState)
	{
	case MaitaCaughtState::NextState::Popped:
		return std::make_unique<MaitaPoppedState>(m_pEnemy);
		break;
	case MaitaCaughtState::NextState::Free:
		return std::make_unique<MaitaRunState>(m_pEnemy, m_pEnemy->GetComponent<EnemyComponent>(), true);
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

		switch (m_pOccupiedBubble->GetShooterType())
		{
		case PlayerComponent::PlayerType::Green:
			m_pSpriteComp->SetRow(m_CaughtInfo.rowNumber);
			break;
		case PlayerComponent::PlayerType::Blue:
			m_pSpriteComp->SetRow(m_CaughtInfo.rowNumber + 1);
			break;
		}
		break;
	case BubbleComponent::FloatingStage::Pink:
		m_pSpriteComp->SetRow(m_CaughtInfo.rowNumber + 2);
		break;
	case BubbleComponent::FloatingStage::Red:
		m_pSpriteComp->SetRow(m_CaughtInfo.rowNumber + 3);
		break;
	}

	m_pSpriteComp->SetNrOfCols(m_CaughtInfo.nrOfCols, false);
	m_pEnemy->GetComponent<FloorCheckingComponent>()->SetHandleCollison(false);
	m_pCollisionComp->SetTag(collisionTags::caughtEnemyTag);
	m_pEnemy->SetLocalPos(-EnemyComponent::GetMaitaOffset(), 0.f);
	m_pPhysicsComp->SetVelocityX(0);
	m_pPhysicsComp->SetVelocityY(0);
	m_pPhysicsComp->StopGravity();

}
void MaitaCaughtState::OnExit()
{
	m_pEnemy->GetComponent<FloorCheckingComponent>()->SetHandleCollison(true);
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
		m_pEnemy->SetParent(nullptr, true);
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
