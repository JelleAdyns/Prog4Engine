#ifndef ZENCHANRUNSTATE_H
#define ZENCHANRUNSTATE_H

#include "ZenChanState.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include <PhysicsComponent.h>
#include <GameObject.h>
#include <Observer.h>

class ZenChanRunState final : public ZenChanState, public dae::Observer<PlayerComponent>
{
public:
	explicit ZenChanRunState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp);
	virtual ~ZenChanRunState() = default;

	ZenChanRunState(const ZenChanRunState&) = delete;
	ZenChanRunState(ZenChanRunState&&) noexcept = delete;
	ZenChanRunState& operator= (const ZenChanRunState&) = delete;
	ZenChanRunState& operator= (ZenChanRunState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
	
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

	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSpriteSubjects;
};


#endif // !ZENCHANRUNSTATE_H