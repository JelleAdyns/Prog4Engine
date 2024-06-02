#include "PickUpComponent.h"
#include "ScoreUIComponent.h"
#include "CollisionTags.h"
#include "spawners.h"
#include <CollisionComponent.h>
#include <RenderComponent.h>
#include <GameObject.h>
#include <GameTime.h>

PickUpComponent::PickUpComponent(dae::GameObject* pOwner, PickUpComponent::PickUpType pickUpType, ScoreUIComponent* pObserver):
	dae::Component{pOwner},
	m_PickUpType{pickUpType},
	m_pCollisionComp{},
	m_pSpriteComp{},
	m_PickedUp{std::make_unique<dae::Subject<PickUpComponent>>()}
{
	m_PickedUp->AddObserver(pObserver);
}

PickUpComponent::~PickUpComponent()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		if (pSubject) pSubject->RemoveObserver(this);
	}
}

void PickUpComponent::Start()
{
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();
	if (!m_pRenderComp) m_pRenderComp = GetOwner()->GetComponent<dae::RenderComponent>();
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();

	switch (m_PickUpType)
	{
	case PickUpComponent::PickUpType::Melon:
		m_pSpriteComp->SetCol(0);
		break;
	case PickUpComponent::PickUpType::Fries:
		m_pSpriteComp->SetCol(1);
		break;
	}

}
 
void PickUpComponent::Update()
{
	HandleTimers();

	//if(m_Timer < m_MaxTimeAlive)
	//{
	//	dae::GameObject* pPlayer = m_pCollisionComp->CheckForCollision(collisionTags::playerTag);
	//	if (pPlayer)
	//	{
	//		m_PickedUp->NotifyObservers(this);
	//		GetOwner()->MarkDead();
	//	}
	//}
}

void PickUpComponent::PrepareImGuiRender()
{
}

void PickUpComponent::Notify(SpriteComponent*)
{
	GetOwner()->MarkDead();
}

void PickUpComponent::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSubjects.emplace_back(pSubject);
}

void PickUpComponent::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		pSubject = nullptr;
	}
}

void PickUpComponent::PickUp(PlayerComponent::PlayerType playerType)
{
	spawners::SpawnFloatingScore(GetOwner()->GetWorldPosition(), m_PickUpType, playerType);
	GetOwner()->MarkDead();
}

PickUpComponent::PickUpType PickUpComponent::GetPickUpType() const
{
	return m_PickUpType;
}

void PickUpComponent::HandleTimers()
{
	if(m_Timer < m_MaxTimeAlive)
	{
		auto deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
		m_Timer += deltaTime;

		if (m_Timer >= m_TimeToFlicker)
		{
			m_FlickerTimer += deltaTime;
			if (m_FlickerTimer >= m_FlickerDelay)
			{
				m_pRenderComp->ToggleNeedToRender();
				m_FlickerTimer = 0.f;
			}
		}
		if (m_Timer >= m_MaxTimeAlive)
		{
			m_pSpriteComp->SetCol(2);
			m_pSpriteComp->SetUpdate(true);
		}
	}

}
