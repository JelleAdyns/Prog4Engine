#ifndef ZENCHANRUNSTATE_H
#define ZENCHANRUNSTATE_H

#include "ZenChanState.h"
#include "PlayerComponent.h"

namespace dae
{
	class GameObject;
	class CollisionComponent;
	class PhysicsComponent;
}

class EnemyComponent;
class SpriteComponent;
class WallCheckingComponent;
class FloorCheckingComponent;
class ZenChanRunState final : public ZenChanState
{
public:
	explicit ZenChanRunState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry = false);
	virtual ~ZenChanRunState() = default;

	ZenChanRunState(const ZenChanRunState&) = delete;
	ZenChanRunState(ZenChanRunState&&) noexcept = delete;
	ZenChanRunState& operator= (const ZenChanRunState&) = delete;
	ZenChanRunState& operator= (ZenChanRunState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
	
	virtual void NotifyPlayerObservers(PlayerComponent* pSubject) override;


private:

	static constexpr float m_GeneralSpeed{40.f};
	const float m_Speed;

	bool m_HasToJump{ false };
	bool m_IsAngry;

	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
	WallCheckingComponent* m_pWallCheckingComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
	dae::CollisionComponent* m_pCollisionComp;


};


#endif // !ZENCHANRUNSTATE_H