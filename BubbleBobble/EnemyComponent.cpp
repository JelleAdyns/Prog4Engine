#include "EnemyComponent.h"
#include "PlayerComponent.h"
#include <CollisionComponent.h>
#include <GameObject.h>

EnemyComponent::EnemyComponent(dae::GameObject* pOwner, PlayerComponent* pObserver):
	dae::Component{pOwner},
	m_pCollided{std::make_unique<dae::Subject<EnemyComponent>>()}
{
	m_pCollided->AddObserver(pObserver);
}

void EnemyComponent::Update()
{
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();

	if (m_pCollisionComp->GetCollisionFlags() > 0) m_pCollided->NotifyObservers(this);
}

void EnemyComponent::PrepareImGuiRender()
{
}
