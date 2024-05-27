#ifndef ZENCHANRUNSTATE_H
#define ZENCHANRUNSTATE_H

#include "ZenChanState.h"
#include "PlayerComponent.h"
#include <Observer.h>

namespace dae
{
	class GameObject;
	class CollisionComponent;
	class PhysicsComponent;
}

class ZenChanComponent;
class SpriteComponent;
class WallCheckingComponent;
class FloorCheckingComponent;
class ZenChanRunState final : public ZenChanState, public dae::Observer<PlayerComponent>
{
public:
	explicit ZenChanRunState(dae::GameObject* pEnemy, ZenChanComponent* pEnemyComp, bool isAngry = false);
	virtual ~ZenChanRunState()
	{
		for (dae::Subject<PlayerComponent>* pSpriteSubject : m_pVecObservedSpriteSubjects)
		{
			if (pSpriteSubject) pSpriteSubject->RemoveObserver(this);
		}
	}

	ZenChanRunState(const ZenChanRunState&) = delete;
	ZenChanRunState(ZenChanRunState&&) noexcept = delete;
	ZenChanRunState& operator= (const ZenChanRunState&) = delete;
	ZenChanRunState& operator= (ZenChanRunState&&) noexcept = delete;

	virtual std::unique_ptr<ZenChanState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
	
	virtual void Notify(PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

private:

	static constexpr float m_GeneralSpeed{40.f};
	const float m_Speed;

	bool m_HasToJump{ false };
	bool m_IsAngry;

	dae::GameObject* m_pEnemy;
	ZenChanComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
	WallCheckingComponent* m_pWallCheckingComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
	dae::CollisionComponent* m_pCollisionComp;

	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSpriteSubjects;
};


#endif // !ZENCHANRUNSTATE_H