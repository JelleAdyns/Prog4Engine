#include "MaitaPoppedState.h"
#include "CollisionTags.h"
#include <AudioLocator.h>
#include "Game.h"


MaitaPoppedState::MaitaPoppedState(dae::GameObject* pEnemy) :
	MaitaState{pEnemy},
	m_pEnemy{ pEnemy },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() },
	m_pFloorComp{ pEnemy->GetComponent<FloorCheckingComponent>() },
	m_pWallComp{ pEnemy->GetComponent<WallCheckingComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() }
{

	dae::AudioLocator::GetAudioService().AddSound("Sounds/EnemyPopped.wav", static_cast<dae::SoundID>(Game::SoundEvent::EnemyDeath));
	dae::AudioLocator::GetAudioService().PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::EnemyDeath), 120, false);
}


std::unique_ptr<EnemyState> MaitaPoppedState::Update()
{
	if (m_pFloorComp->IsOnGround())
	{
		auto pos = m_pEnemy->GetWorldPosition();
		pos.x += EnemyComponent::GetMaitaOffset();
		spawners::SpawnPickUp(pos, PickUpComponent::PickUpType::Fries);
		m_pEnemy->MarkDead();
	}
	if (m_pWallComp->CollidingWithLeft()) m_pPhysicsComp->SetVelocityX(m_Speed);
	if (m_pWallComp->CollidingWithRight()) m_pPhysicsComp->SetVelocityX(-m_Speed);
	return nullptr;
}
void MaitaPoppedState::OnEnter()
{
	if (IsPlayable())
	{
		MovementComponent* pMoveComp = m_pEnemy->GetComponent<MovementComponent>();
		pMoveComp->UnRegisterAttackCommand();
		pMoveComp->UnRegisterJumpCommand();
		pMoveComp->UnRegisterMoveCommands();
	}

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

