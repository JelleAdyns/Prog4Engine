#include "MaitaPoppedState.h"
#include "CollisionTags.h"


MaitaPoppedState::MaitaPoppedState(dae::GameObject* pMaita) :
	MaitaState{},
	m_pMaita{ pMaita },
	m_pPhysicsComp{ pMaita->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pMaita->GetComponent<dae::CollisionComponent>() },
	m_pFloorComp{ pMaita->GetComponent<FloorCheckingComponent>() },
	m_pWallComp{ pMaita->GetComponent<WallCheckingComponent>() },
	m_pSpriteComp{ pMaita->GetComponent<SpriteComponent>() }
{}

std::unique_ptr<MaitaState> MaitaPoppedState::Update()
{
	if (m_pFloorComp->IsOnGround())
	{
		auto pos = m_pMaita->GetWorldPosition();
		pos.x += MaitaComponent::GetMaitaOffset();
		spawners::SpawnPickUp(pos, PickUpComponent::PickUpType::Fries);
		m_pMaita->MarkDead();
	}
	if (m_pWallComp->CollidingWithLeft()) m_pPhysicsComp->SetVelocityX(m_Speed);
	if (m_pWallComp->CollidingWithRight()) m_pPhysicsComp->SetVelocityX(-m_Speed);
	return nullptr;
}
void MaitaPoppedState::OnEnter()
{
	//m_pCollisionComp->SetCollision(false);
	m_pCollisionComp->SetTag(collisionTags::caughtEnemyTag);
	m_pSpriteComp->SetRow(m_PoppedInfo.rowNumber);
	m_pSpriteComp->SetFrameTime(m_PoppedInfo.frameTime);

	m_pPhysicsComp->SetVelocityX((rand() + 1) % 2 ? m_Speed : -m_Speed);
	m_pPhysicsComp->SetVelocityY(-m_Speed * 1.25f);
}

void MaitaPoppedState::OnExit()
{

}