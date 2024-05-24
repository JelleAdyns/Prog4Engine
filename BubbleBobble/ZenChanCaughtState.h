#ifndef ZENCHANCAUGHTSTATE_H
#define ZENCHANCAUGHTSTATE_H

#include "ZenChanState.h"

#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include <PhysicsComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>


class ZenChanCaughtState final : public ZenChanState
{
public:
	explicit ZenChanCaughtState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp) :
		ZenChanState{},
		m_pEnemy{ pEnemy },
		m_pEnemyComp{ pEnemyComp },
		m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
		m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() },
		m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() }
	{};
	virtual ~ZenChanCaughtState() = default;

	ZenChanCaughtState(const ZenChanCaughtState&) = delete;
	ZenChanCaughtState(ZenChanCaughtState&&) noexcept = delete;
	ZenChanCaughtState& operator= (const ZenChanCaughtState&) = delete;
	ZenChanCaughtState& operator= (ZenChanCaughtState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override
	{

		return nullptr;
	}
	virtual void OnEnter() override
	{

	}
	virtual void OnExit() override
	{



	}

private:

	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
	SpriteComponent* m_pSpriteComp;
};


#endif // !ZENCHANCAUGHTSTATE_H