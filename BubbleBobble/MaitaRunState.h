#ifndef MAITARUNSTATE_H
#define MAITARUNSTATE_H

#include "ZenChanState.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include <PhysicsComponent.h>
#include <GameObject.h>

class MaitaRunState final : public ZenChanState
{
public:
	explicit MaitaRunState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp) :
		ZenChanState{},
		m_pEnemy{ pEnemy },
		m_pEnemyComp{ pEnemyComp },
		m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() }
	{}
	virtual ~MaitaRunState() = default;

	MaitaRunState(const MaitaRunState&) = delete;
	MaitaRunState(MaitaRunState&&) noexcept = delete;
	MaitaRunState& operator= (const MaitaRunState&) = delete;
	MaitaRunState& operator= (MaitaRunState&&) noexcept = delete;

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
};


#endif // !MAITARUNSTATE_H