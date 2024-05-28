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
	explicit ZenChanPoppedState(dae::GameObject* pEnemy);
	virtual ~ZenChanPoppedState() = default;

	ZenChanPoppedState(const ZenChanPoppedState&) = delete;
	ZenChanPoppedState(ZenChanPoppedState&&) noexcept = delete;
	ZenChanPoppedState& operator= (const ZenChanPoppedState&) = delete;
	ZenChanPoppedState& operator= (ZenChanPoppedState&&) noexcept = delete;

	virtual std::unique_ptr<ZenChanState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:

	static constexpr SpriteComponent::SpriteInfo m_PoppedInfo{ .rowNumber{2}, .frameTime{0.06f}};

	const float m_Speed{ 120.f };

	dae::GameObject* m_pEnemy;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	FloorCheckingComponent* m_pFloorComp;
	WallCheckingComponent* m_pWallComp;
	SpriteComponent* m_pSpriteComp;

};


#endif // !ZENCHANPOPPEDSTATE_H