#include "BubbleComponent.h"
#include "WallCheckingComponent.h"
#include "SpriteComponent.h"
#include "CollisionTags.h"
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <RenderComponent.h>
#include <GameTime.h>
#include <algorithm>

BubbleComponent::BubbleComponent(dae::GameObject* pOwner, PlayerComponent::PlayerType shooter, bool left):
	dae::Component{pOwner},
	m_Shooter{shooter},
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
		if(pSubject) pSubject->RemoveObserver(this);
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
	switch (m_Shooter)
	{
	case PlayerComponent::PlayerType::Green:
		m_pSpriteComp->SetRow(0);
		break;
	case PlayerComponent::PlayerType::Blue:
		m_pSpriteComp->SetRow(3);
		break;
	}
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
			m_pSpriteComp->AddRows(1);
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

void BubbleComponent::SetSubjectPointersInvalid(dae::Subject<SpriteComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSpriteSubjects.begin(), m_pVecObservedSpriteSubjects.end(), pSubject);
	if (pos != m_pVecObservedSpriteSubjects.cend())
	{
		m_pVecObservedSpriteSubjects.erase(pos);
	}
}

void BubbleComponent::AddObserver(dae::Observer<BubbleComponent>* pObserver)
{
	m_pTimerTick->AddObserver(pObserver);
}

void BubbleComponent::SetOccupied()
{
	m_pCollisionComp->SetTag(collisionTags::OccupiedBubbleTag);
	m_IsOccupied = true;
}


bool BubbleComponent::IsPoppedByPlayer()
{
	return m_PoppedByPlayer;
}

BubbleComponent::FloatingStage BubbleComponent::GetFloatingStage() const
{
	return m_FloatingStage;
}

bool BubbleComponent::IsFloating() const
{
	return m_CurrState == BubbleState::Floating;
}

PlayerComponent::PlayerType BubbleComponent::GetShooterType() const
{
	return m_Shooter;
}

void BubbleComponent::HandleShotState()
{
	
	static constexpr SpriteComponent::SpriteInfo shotInfo{ .rowUpdate{true}, .frameTime{0.1f } };

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
	static constexpr SpriteComponent::SpriteInfo floatInfo{ .rowUpdate{false}, .rowNumber{6}, .frameTime{0.1f} };

	m_pSpriteComp->SetRowUpdate(floatInfo.rowUpdate);
	m_pSpriteComp->SetFrameTime(floatInfo.frameTime);

	m_pPhysicsComp->SetVelocityX(0.f);
	m_pPhysicsComp->SetVelocityY(m_YVelocity);

	if (GetOwner()->GetWorldPosition().y < m_MaxHeight) m_pPhysicsComp->SetVelocityY(0);

	const static float offset{ 3.f };
	m_pCollisionComp->CheckForCollision(glm::vec2{0, offset}, glm::vec2{ m_pSpriteComp->GetDestRectSize().x, m_pSpriteComp->GetDestRectSize().y - offset}, collisionTags::playerTag);
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

