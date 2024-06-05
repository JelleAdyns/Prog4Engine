#ifndef ZENCHANJUMPSTATE_H
#define ZENCHANJUMPSTATE_H

#include "ZenChanState.h"

namespace dae
{
	class GameObject;
	class PhysicsComponent;
	class CollisionComponent;
}

class EnemyComponent;
class SpriteComponent;
class ZenChanJumpState final : public ZenChanState
{
public:
	explicit ZenChanJumpState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry);
	virtual ~ZenChanJumpState() = default;

	ZenChanJumpState(const ZenChanJumpState&) = delete;
	ZenChanJumpState(ZenChanJumpState&&) noexcept = delete;
	ZenChanJumpState& operator= (const ZenChanJumpState&) = delete;
	ZenChanJumpState& operator= (ZenChanJumpState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void NotifyPlayerObservers(PlayerComponent* pSubject) override;
private:

	bool m_IsAngry;
	bool m_CheckIfLanded{ false };

	float m_TimeToJump;
	float m_TimeBeforeJump{};
	float m_TimeBeforeFlip{};

	const float m_JumpVelocity{ -160.f };


	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
	dae::CollisionComponent* m_pCollisionComp;
};


#endif // !ZENCHANJUMPSTATE_H