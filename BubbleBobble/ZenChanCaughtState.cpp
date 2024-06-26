#include "ZenChanCaughtState.h"
#include "ZenChanPoppedState.h"
#include "EnemyComponent.h"
#include "BubbleComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>
#include "ZenChanRunState.h"
#include "CollisionTags.h"

ZenChanCaughtState::ZenChanCaughtState(dae::GameObject* pEnemy, dae::GameObject* pBubble) :
	ZenChanState{},
	m_pEnemy{ pEnemy },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
	m_pOccupiedBubble{ pBubble->GetComponent<BubbleComponent>() },
	m_pObservedSubject{ nullptr }
{
	assert(pBubble->HasComponent<BubbleComponent>() && "The passed GameObject didn't have a BubbleComponent. Not good!");
	m_pEnemy->SetParent(pBubble, false);

	dae::RenderComponent* pRenderComp = pBubble->GetComponent<dae::RenderComponent>();
	pRenderComp->SetNeedToRender(false);

};


std::unique_ptr<EnemyState> ZenChanCaughtState::Update()
{
	switch (m_NextState)
	{
	case ZenChanCaughtState::NextState::Popped:
		return std::make_unique<ZenChanPoppedState>(m_pEnemy);
		break;
	case ZenChanCaughtState::NextState::Free:
		return std::make_unique<ZenChanRunState>(m_pEnemy, m_pEnemy->GetComponent<EnemyComponent>(), true);
		break;
	}

	return nullptr;
}
void ZenChanCaughtState::OnEnter()
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



	m_pEnemy->GetComponent<FloorCheckingComponent>()->SetHandleCollison(false);
	m_pCollisionComp->SetTag(collisionTags::caughtEnemyTag);
	m_pCollisionComp->SetCollision(false);
	m_pEnemy->SetLocalPos(0.f, 0.f);
	m_pPhysicsComp->SetVelocityX(0);
	m_pPhysicsComp->SetVelocityY(0);
	m_pPhysicsComp->StopGravity();

}
void ZenChanCaughtState::OnExit()
{
	m_pEnemy->GetComponent<FloorCheckingComponent>()->SetHandleCollison(true);
	m_pCollisionComp->SetTag(collisionTags::enemyTag);
	m_pCollisionComp->SetCollision(true);

	m_pPhysicsComp->StartGravity();
}

void ZenChanCaughtState::NotifyPlayerObservers(PlayerComponent*)
{
}

void ZenChanCaughtState::Notify(BubbleComponent* pSubject)
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
		if(pSubject->IsPoppedByPlayer()) m_NextState = NextState::Popped;
		else m_NextState = NextState::Free;
		break;
	}
}
void ZenChanCaughtState::AddSubjectPointer(dae::Subject<BubbleComponent>* pSubject)
{
	m_pObservedSubject = pSubject;
}

void ZenChanCaughtState::SetSubjectPointersInvalid(dae::Subject<BubbleComponent>*)
{
	m_pObservedSubject = nullptr;
}
