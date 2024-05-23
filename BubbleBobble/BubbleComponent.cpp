#include "BubbleComponent.h"
#include "EnemyComponent.h"
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <GameTime.h>

BubbleComponent::BubbleComponent(dae::GameObject* pOwner):
	dae::Component{pOwner},
	m_CurrState{BubbleState::Shot},
	m_pSpriteComp{},
	m_pVecObservedSpriteSubjects{},
	m_pVecObservedEnemySubjects{}
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
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	m_pSpriteComp->AddObserver(this);
	m_pPhysicsComp->StopGravity();
}

void BubbleComponent::Update()
{
	switch (m_CurrState)
	{
	case BubbleState::Shot:
		m_pSpriteComp->SetRowUpdate(true);
		m_pPhysicsComp->SetVelocityY(0.f);
		m_pPhysicsComp->SetVelocityX(20.f);
		break;
	case BubbleState::Floating:
	{
		m_pPhysicsComp->SetVelocityX(0.f);
		m_pPhysicsComp->SetVelocityY(-20.f);
		static const int startRow{ 6 };
		m_TimeBeforePop += dae::GameTime::GetInstance().GetDeltaTime();
		if (m_TimeBeforePop >= m_TimeToPop / 3.f) m_pSpriteComp->SetRow(startRow);
		if (m_TimeBeforePop >= m_TimeToPop / 3.f * 2) m_pSpriteComp->AddRows(1);
		if (m_TimeBeforePop >= m_TimeToPop)
		{
			m_CurrState = BubbleState::Popped;
			m_pSpriteComp->AddRows(1);
		}
	}
		break;
	case BubbleState::Popped:
		m_pSpriteComp->SetRowUpdate(true);
		m_pPhysicsComp->SetVelocityY(0.f);
		break;
	}
}

void BubbleComponent::PrepareImGuiRender()
{
}

void BubbleComponent::Notify(EnemyComponent*)
{
}

void BubbleComponent::AddSubjectPointer(dae::Subject<EnemyComponent>* pSubject)
{
	m_pVecObservedEnemySubjects.push_back(pSubject);
}

void BubbleComponent::Notify(SpriteComponent* )
{
	static int row{};
	++row;
	switch (m_CurrState)
	{
	case BubbleState::Shot:

		if (row == 2)
		{
			row = 0;
			m_CurrState = BubbleState::Floating;
			m_pSpriteComp->SetRowUpdate(false);
			m_pSpriteComp->SetRow(2);
		}
		break;
	case BubbleState::Floating:
		

		break;
	case BubbleState::Popped:
		break;
	}
}

void BubbleComponent::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.push_back(pSubject);
}