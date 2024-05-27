#include "PickUpComponent.h"
#include "ScoreUIComponent.h"
#include "CollisionTags.h"
#include <CollisionComponent.h>
#include <GameObject.h>

PickUpComponent::PickUpComponent(dae::GameObject* pOwner, PickUpComponent::PickUpType pickUpType, ScoreUIComponent* pObserver):
	dae::Component{pOwner},
	m_PickUpType{pickUpType},
	m_PickedUp{std::make_unique<dae::Subject<PickUpComponent>>()}
{
	m_PickedUp->AddObserver(pObserver);
}

void PickUpComponent::Start()
{
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();
}

void PickUpComponent::Update()
{
	m_pCollisionComp->CheckForCollision(collisionTags::playerTag);
	
}

void PickUpComponent::PrepareImGuiRender()
{
}

void PickUpComponent::PickUp()
{
	m_PickedUp->NotifyObservers(this);
	GetOwner()->MarkDead();
}

PickUpComponent::PickUpType PickUpComponent::GetPickUpType() const
{
	return m_PickUpType;
}
