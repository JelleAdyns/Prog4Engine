#ifndef ZENCHANANGRYSTATE_H
#define ZENCHANANGRYSTATE_H

#include "ZenChanState.h"
#include "PlayerComponent.h"
#include <Observer.h>

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
class ZenChanAngryState final : public ZenChanState, public dae::Observer<PlayerComponent>
{
public:
	explicit ZenChanAngryState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp);
	virtual ~ZenChanAngryState() = default;

	ZenChanAngryState(const ZenChanAngryState&) = delete;
	ZenChanAngryState(ZenChanAngryState&&) noexcept = delete;
	ZenChanAngryState& operator= (const ZenChanAngryState&) = delete;
	ZenChanAngryState& operator= (ZenChanAngryState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override
	{

	}
	virtual void OnEnter() override
	{

	}
	virtual void OnExit() override
	{

	}

	virtual void Notify(PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;

private:

	bool m_HasToJump{ false };

	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
	WallCheckingComponent* m_pWallCheckingComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
	dae::CollisionComponent* m_pCollisionComp;

	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSpriteSubjects;
};


#endif // !ZENCHANANGRYSTATE_H