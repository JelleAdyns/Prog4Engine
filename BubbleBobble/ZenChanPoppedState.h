#ifndef ZENCHANPOPPEDSTATE_H
#define ZENCHANPOPPEDSTATE_H

#include "ZenChanState.h"
#include "Spawners.h"
#include "SpriteComponent.h"
#include "FloorCheckingComponent.h"
#include "WallCheckingComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <GameTime.h>
#include <Observer.h>


class ZenChanPoppedState final : public ZenChanState
{
public:
	explicit ZenChanPoppedState(dae::GameObject* pEnemy):
	ZenChanState{},
		m_pEnemy{ pEnemy },
		m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
		m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() },
		m_pFloorComp{ pEnemy->GetComponent<FloorCheckingComponent>() },
		m_pWallComp{ pEnemy->GetComponent<WallCheckingComponent>() },
		m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() }
	{}
	virtual ~ZenChanPoppedState() = default;

	ZenChanPoppedState(const ZenChanPoppedState&) = delete;
	ZenChanPoppedState(ZenChanPoppedState&&) noexcept = delete;
	ZenChanPoppedState& operator= (const ZenChanPoppedState&) = delete;
	ZenChanPoppedState& operator= (ZenChanPoppedState&&) noexcept = delete;

	virtual std::unique_ptr<ZenChanState> Update() override
	{
		if (m_pFloorComp->IsOnGround())
		{
			spawners::SpawnPickUp(m_pEnemy->GetWorldPosition(), PickUpComponent::PickUpType::Melon);
			m_pEnemy->MarkDead();
		}
		if (m_pWallComp->CollidingWithLeft()) m_pPhysicsComp->SetVelocityX(m_Speed);
		if (m_pWallComp->CollidingWithRight()) m_pPhysicsComp->SetVelocityX(-m_Speed);
		return nullptr;
	}
	virtual void OnEnter() override
	{
		//m_pCollisionComp->SetCollision(false);
		m_pCollisionComp->SetTag(collisionTags::caughtEnemyTag);
		m_pSpriteComp->SetRow(m_PoppedInfo.rowNumber);
		m_pSpriteComp->SetFrameTime(m_PoppedInfo.frameTime);

		m_pPhysicsComp->SetVelocityX((rand() + 1) % 2 ? m_Speed : -m_Speed);
		m_pPhysicsComp->SetVelocityY(-m_Speed);
	}
	
	virtual void OnExit() override
	{

	}

private:

	static constexpr SpriteComponent::RowInfo m_PoppedInfo{ .rowNumber{2}, .frameTime{0.06f}};

	float m_Speed{ 120.f };

	dae::GameObject* m_pEnemy;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	FloorCheckingComponent* m_pFloorComp;
	WallCheckingComponent* m_pWallComp;
	SpriteComponent* m_pSpriteComp;

};


#endif // !ZENCHANPOPPEDSTATE_H