#include "PickUpComponent.h"
#include "ScoreUIComponent.h"
#include "SpriteComponent.h"
#include "CollisionTags.h"
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
	dae::GameObject* pPlayer = m_pCollisionComp->CheckForCollision(collisionTags::playerTag);
	if(pPlayer)
	{
		m_PickedUp->NotifyObservers(this);
		GetOwner()->MarkDead();
	}

	HandleTimers();
}

void PickUpComponent::PrepareImGuiRender()
{
}

PickUpComponent::PickUpType PickUpComponent::GetPickUpType() const
{
	return m_PickUpType;
}

void PickUpComponent::HandleTimers()
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
	if (m_Timer >= m_MaxTimeAlive) GetOwner()->MarkDead();

}
