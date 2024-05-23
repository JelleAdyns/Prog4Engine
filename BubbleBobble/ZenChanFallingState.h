#ifndef ZENCHANFALLINGSTATE_H
#define ZENCHANFALLINGSTATE_H

#include "ZenChanState.h"

#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include <PhysicsComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <Observer.h>


class ZenChanFallingState final : public ZenChanState, public dae::Observer<PlayerComponent>
{
public:
	explicit ZenChanFallingState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp);
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

	float m_PlayerXPos{};

	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	FloorCheckingComponent* m_pFloorCheckingComp;


	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSubjects;
};


#endif // !ZENCHANFALLINGSTATE_H