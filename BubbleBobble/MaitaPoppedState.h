#ifndef MAITAPOPPEDSTATE_H
#define MAITAPOPPEDSTATE_H

#include "MaitaState.h"
#include "Spawners.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include "FloorCheckingComponent.h"
#include "WallCheckingComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <GameTime.h>
#include <Observer.h>


class MaitaPoppedState final : public MaitaState
{
public:
	explicit MaitaPoppedState(dae::GameObject* pEnemy);
	virtual ~MaitaPoppedState() = default;

	MaitaPoppedState(const MaitaPoppedState&) = delete;
	MaitaPoppedState(MaitaPoppedState&&) noexcept = delete;
	MaitaPoppedState& operator= (const MaitaPoppedState&) = delete;
	MaitaPoppedState& operator= (MaitaPoppedState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void NotifyPlayerObservers(PlayerComponent* pSubject) override;
private:

	static constexpr SpriteComponent::SpriteInfo m_PoppedInfo{ .rowNumber{4}, .frameTime{0.06f} };

	const float m_Speed{ 120.f };

	dae::GameObject* m_pEnemy;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	FloorCheckingComponent* m_pFloorComp;
	WallCheckingComponent* m_pWallComp;
	SpriteComponent* m_pSpriteComp;

};


#endif // !MAITAPOPPEDSTATE_H