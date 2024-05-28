#include "ZenChanCaughtState.h"
#include "ZenChanPoppedState.h"
#include "ZenChanComponent.h"
#include "FloorCheckingComponent.h"
#include "BubbleComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>

ZenChanCaughtState::ZenChanCaughtState(dae::GameObject* pZenChan, dae::GameObject* pBubble) :
	ZenChanState{},
	m_pZenChan{ pZenChan },
	m_pPhysicsComp{ pZenChan->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pZenChan->GetComponent<dae::CollisionComponent>() },
	m_pSpriteComp{ pZenChan->GetComponent<SpriteComponent>() },
	m_pObservedSubject{ nullptr }
{
	assert(pBubble->HasComponent<BubbleComponent>() && "The passed GameObject didn't have a BubbleComponent. Not good!");
	m_pZenChan->SetParent(pBubble, false);

	dae::RenderComponent* pRenderComp = pBubble->GetComponent<dae::RenderComponent>();
	pRenderComp->SetNeedToRender(false);

	BubbleComponent* pBubbleComp = pBubble->GetComponent<BubbleComponent>();
	pBubbleComp->AddObserver(this);
	pBubbleComp->SetOccupied();


	switch (pBubbleComp->GetFloatingStage())
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

};


std::unique_ptr<ZenChanState> ZenChanCaughtState::Update()
{
	switch (m_NextState)
	{
	case ZenChanCaughtState::NextState::Popped:
		return std::make_unique<ZenChanPoppedState>(m_pZenChan);
		break;
	case ZenChanCaughtState::NextState::Free:
		return std::make_unique<ZenChanRunState>(m_pZenChan, m_pZenChan->GetComponent<ZenChanComponent>(), true);
		break;
	}

	return nullptr;
}
void ZenChanCaughtState::OnEnter()
{
	m_pZenChan->GetComponent<FloorCheckingComponent>()->SetHandleCollison(false);
	m_pCollisionComp->SetTag(collisionTags::caughtEnemyTag);
	m_pZenChan->SetLocalPos(0.f, 0.f);
	m_pPhysicsComp->SetVelocityX(0);
	m_pPhysicsComp->SetVelocityY(0);
	m_pPhysicsComp->StopGravity();

}
void ZenChanCaughtState::OnExit()
{
	m_pZenChan->GetComponent<FloorCheckingComponent>()->SetHandleCollison(true);
	m_pCollisionComp->SetTag(collisionTags::enemyTag);

	m_pPhysicsComp->StartGravity();
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
		m_pZenChan->SetParent(nullptr, true);
		if(pSubject->IsPoppedByPlayer()) m_NextState = NextState::Popped;
		else m_NextState = NextState::Free;
		break;
	}
}
void ZenChanCaughtState::AddSubjectPointer(dae::Subject<BubbleComponent>* pSubject)
{
	m_pObservedSubject = pSubject;
}

void ZenChanCaughtState::SetSubjectPointersInvalid()
{
	m_pObservedSubject = nullptr;
}
