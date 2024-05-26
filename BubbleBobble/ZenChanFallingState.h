#ifndef ZENCHANFALLINGSTATE_H
#define ZENCHANFALLINGSTATE_H

#include "ZenChanState.h"
#include "PlayerComponent.h"
#include <Observer.h>

namespace dae
{
	class GameObject;
	class PhysicsComponent;
	class CollisionComponent;
}

class EnemyComponent;
class SpriteComponent;
class FloorCheckingComponent;
class ZenChanFallingState final : public ZenChanState, public dae::Observer<PlayerComponent>
{
public:
	explicit ZenChanFallingState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry);
	virtual ~ZenChanFallingState() = default;

	ZenChanFallingState(const ZenChanFallingState&) = delete;
	ZenChanFallingState(ZenChanFallingState&&) noexcept = delete;
	ZenChanFallingState& operator= (const ZenChanFallingState&) = delete;
	ZenChanFallingState& operator= (ZenChanFallingState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Notify(PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;

private:

	bool m_IsAngry;
	float m_PlayerXPos{};

	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	FloorCheckingComponent* m_pFloorCheckingComp;


	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSubjects;
};


#endif // !ZENCHANFALLINGSTATE_H