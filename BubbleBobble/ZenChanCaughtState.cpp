#include "ZenChanCaughtState.h"
#include "ZenChanRunState.h"

ZenChanCaughtState::ZenChanCaughtState(dae::GameObject* pEnemy, dae::GameObject* pBubble) :
	ZenChanState{},
	m_pEnemy{ pEnemy },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
	m_pObservedSubject{nullptr}
{
	assert(pBubble->HasComponent<BubbleComponent>() && "The passed GameObject didn't have a BubbleComponent. Not good!");
	m_pEnemy->SetParent(pBubble, false);

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


std::unique_ptr<EnemyState> ZenChanCaughtState::Update()
{
	if (m_Pop)
		return std::make_unique<ZenChanRunState>(m_pEnemy, m_pEnemy->GetComponent<EnemyComponent>());
	return nullptr;
}
void ZenChanCaughtState::OnEnter()
{
	m_pEnemy->GetComponent<FloorCheckingComponent>()->SetHandleCollison(false);
	m_pEnemy->SetLocalPos(0.f, 0.f);
	m_pPhysicsComp->SetVelocityX(0);
	m_pPhysicsComp->SetVelocityY(0);
	m_pPhysicsComp->StopGravity();

}
void ZenChanCaughtState::OnExit()
{
	m_pEnemy->GetComponent<FloorCheckingComponent>()->SetHandleCollison(true);

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
		m_pEnemy->SetParent(nullptr, true);
		m_Pop = true;
		break;
	}
}
void ZenChanCaughtState::AddSubjectPointer(dae::Subject<BubbleComponent>* pSubject)
{
	m_pObservedSubject = pSubject;
}