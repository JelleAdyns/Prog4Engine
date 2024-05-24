#include "BubbleComponent.h"
#include "EnemyComponent.h"
#include "WallCheckingComponent.h"
#include "SpriteComponent.h"
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <GameTime.h>

BubbleComponent::BubbleComponent(dae::GameObject* pOwner, bool left):
	dae::Component{pOwner},
	m_CurrState{BubbleState::Shot},
	m_Left{left},
	m_pSpriteComp{},
	m_pWallComp{},
	m_pPhysicsComp{},
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
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	m_pSpriteComp->AddObserver(this);
	m_pPhysicsComp->StopGravity();
}

void BubbleComponent::Update()
{
	switch (m_CurrState)
	{
	case BubbleState::Shot:
	{
		static constexpr SpriteComponent::RowInfo shotInfo{ .rowUpdate{true}, .rowNumber{0}, .frameTime{0.2f } };

		m_pSpriteComp->SetRow(shotInfo.rowNumber);
		m_pSpriteComp->SetRowUpdate(shotInfo.rowUpdate);
		m_pSpriteComp->SetFrameTime(shotInfo.frameTime);

		m_pPhysicsComp->SetVelocityY(0.f);
		m_pPhysicsComp->SetVelocityX(m_Left ? -m_XVelocity : m_XVelocity);

		if (m_pWallComp->CollidingWithLeft() || m_pWallComp->CollidingWithRight())
		{
			m_pSpriteComp->SetRow(8);
			m_CurrState = BubbleState::Popped;
		}
	}
		break;
	case BubbleState::Floating:
	{
		static constexpr SpriteComponent::RowInfo floatInfo{ .rowUpdate{false}, .rowNumber{6}, .frameTime{0.1f} };

		m_pSpriteComp->SetRowUpdate(floatInfo.rowUpdate);
		m_pSpriteComp->SetFrameTime(floatInfo.frameTime);

		m_pPhysicsComp->SetVelocityX(0.f);
		m_pPhysicsComp->SetVelocityY(-20.f);

		if (GetOwner()->GetWorldPosition().y < m_MaxHeight) m_pPhysicsComp->SetVelocityY(0);


		m_TimeBeforePop += dae::GameTime::GetInstance().GetDeltaTime();

		if (m_TimeBeforePop >= m_TimeToPop / 3.f) m_pSpriteComp->SetRow(floatInfo.rowNumber);
		if (m_TimeBeforePop >= m_TimeToPop / 3.f * 2) m_pSpriteComp->AddRows(1);
		if (m_TimeBeforePop >= m_TimeToPop)
		{
			m_CurrState = BubbleState::Popped;
			m_pSpriteComp->AddRows(1);
		}
	}
		break;
	case BubbleState::Popped:
		m_pPhysicsComp->SetVelocityX(0.f);
		m_pSpriteComp->SetRowUpdate(true);
		m_pPhysicsComp->SetVelocityY(0.f);
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

