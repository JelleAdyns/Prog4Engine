#include "BubbleComponent.h"
#include "EnemyComponent.h"
#include "WallCheckingComponent.h"
#include "SpriteComponent.h"
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <RenderComponent.h>
#include <GameTime.h>

BubbleComponent::BubbleComponent(dae::GameObject* pOwner, bool left):
	dae::Component{pOwner},
	m_CurrState{BubbleState::Shot},
	m_Left{left},
	m_pSpriteComp{},
	m_pWallComp{},
	m_pCollisionComp{},
	m_pPhysicsComp{},
	m_pRenderComp{},
	m_pTimerTick{std::make_unique<dae::Subject<BubbleComponent>>()},
	m_pVecObservedSpriteSubjects{}
{
}

BubbleComponent::~BubbleComponent()
{
	for (auto& pSubject : m_pVecObservedSpriteSubjects)
	{
		pSubject->RemoveObserver(this);
	}
}

void BubbleComponent::Start()
{
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();
	if (!m_pWallComp) m_pWallComp = GetOwner()->GetComponent<WallCheckingComponent>();
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	if (!m_pRenderComp) m_pRenderComp = GetOwner()->GetComponent<dae::RenderComponent>();
	m_pSpriteComp->AddObserver(this);
	m_pPhysicsComp->StopGravity();
}

void BubbleComponent::Update()
{
	switch (m_CurrState)
	{
	case BubbleState::Shot:
		HandleShotState();
		break;
	case BubbleState::Floating:
		HandleFloatingState();
		break;
	case BubbleState::Popped:
		HandlePoppedState();
		break;
	}
}

void BubbleComponent::PrepareImGuiRender()
{
}

void BubbleComponent::Notify(SpriteComponent* )
{
	
	
	switch (m_CurrState)
	{
	case BubbleState::Shot:
	{
		++m_RowCount;
		if (m_RowCount == 2)
		{
			m_RowCount = 0;
			m_CurrState = BubbleState::Floating;
			m_pSpriteComp->SetRowUpdate(false);
			m_pSpriteComp->SetRow(2);
		}
	}
	break;
	case BubbleState::Floating:


		break;
	case BubbleState::Popped:
	{
		++m_RowCount;
		if (m_RowCount == 3)
		{
			m_RowCount = 0;
			GetOwner()->MarkDead();
		}
	}
		break;
	}
}

void BubbleComponent::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.push_back(pSubject);
}

void BubbleComponent::AddObserver(dae::Observer<BubbleComponent>* pObserver)
{
	m_pTimerTick->AddObserver(pObserver);
}

void BubbleComponent::SetOccupied()
{
	m_IsOccupied = true;
}

bool BubbleComponent::IsOccupied()
{
	return m_IsOccupied;
}

bool BubbleComponent::IsPoppedByPlayer()
{
	return m_PoppedByPlayer;
}

BubbleComponent::FloatingStage BubbleComponent::GetFloatingStage() const
{
	return m_FloatingStage;
}

void BubbleComponent::HandleShotState()
{
	
	static constexpr SpriteComponent::RowInfo shotInfo{ .rowUpdate{true}, .frameTime{0.1f } };

	m_pSpriteComp->SetRowUpdate(shotInfo.rowUpdate);
	m_pSpriteComp->SetFrameTime(shotInfo.frameTime);

	m_pPhysicsComp->SetVelocityY(0.f);
	m_pPhysicsComp->SetVelocityX(m_Left ? -m_XSpeed : m_XSpeed);

	if (m_IsOccupied)
	{
		m_CurrState = BubbleState::Floating;
		m_pSpriteComp->SetRowUpdate(false);
		m_pSpriteComp->SetRow(2);
	}

	if (m_pWallComp->CollidingWithLeft() || m_pWallComp->CollidingWithRight())
	{
		m_pSpriteComp->SetRow(8);
		m_pSpriteComp->SetCol(0);
		m_RowCount = 0;
		m_CurrState = BubbleState::Popped;
		m_FloatingStage = FloatingStage::Red;
		m_pTimerTick->NotifyObservers(this);
	}
	
}

void BubbleComponent::HandleFloatingState()
{
	static constexpr SpriteComponent::RowInfo floatInfo{ .rowUpdate{false}, .rowNumber{6}, .frameTime{0.1f} };

	m_pSpriteComp->SetRowUpdate(floatInfo.rowUpdate);
	m_pSpriteComp->SetFrameTime(floatInfo.frameTime);

	m_pPhysicsComp->SetVelocityX(0.f);
	m_pPhysicsComp->SetVelocityY(m_YVelocity);

	if (GetOwner()->GetWorldPosition().y < m_MaxHeight) m_pPhysicsComp->SetVelocityY(0);


	m_pCollisionComp->CheckForCollision(collisionTags::playerTag);
	if (m_pCollisionComp->GetCollisionFlags() > 0)
	{
		m_PoppedByPlayer = true;
		m_CurrState = BubbleState::Popped;
		m_FloatingStage = FloatingStage::Red;
		m_pSpriteComp->SetRow(8);
		m_pTimerTick->NotifyObservers(this);	
	}

	m_TimeBeforePop += dae::GameTime::GetInstance().GetDeltaTime();


	switch (m_FloatingStage)
	{
	case FloatingStage::ShooterColor:
		if (m_TimeBeforePop >= m_TimeToPop / 3.f)
		{
			m_pSpriteComp->SetRow(floatInfo.rowNumber);
			m_pTimerTick->NotifyObservers(this);
			m_FloatingStage = FloatingStage::Pink;
		}
		break;
	case FloatingStage::Pink:
		if (m_TimeBeforePop >= m_TimeToPop / 3.f * 2)
		{
			m_pSpriteComp->AddRows(1);
			m_pTimerTick->NotifyObservers(this);
			m_FloatingStage = FloatingStage::Red;
		}
		break;
	case FloatingStage::Red:
		if (m_TimeBeforePop >= m_TimeToPop)
		{
			m_pTimerTick->NotifyObservers(this);
			m_CurrState = BubbleState::Popped;
			m_pSpriteComp->AddRows(1);
		}
		break;
	}
}

void BubbleComponent::HandlePoppedState()
{
	m_pRenderComp->SetNeedToRender(true);
	m_pCollisionComp->SetCollision(false);
	m_pPhysicsComp->SetVelocityX(0.f);
	m_pPhysicsComp->SetVelocityY(0.f);
	m_pSpriteComp->SetRowUpdate(true);
}

