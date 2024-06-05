#ifndef MAITASTATE_H
#define MAITASTATE_H

#include <memory>
#include <GameObject.h>
#include "EnemyState.h"
#include "MovementComponent.h"

class PlayerComponent;
class MaitaState: public EnemyState
{
public:
	MaitaState(dae::GameObject* pEnemy) :
		EnemyState{},
		m_IsPlayable{pEnemy->HasComponent<MovementComponent>()}
	{}
	virtual ~MaitaState() = default;

	MaitaState(const MaitaState&) = delete;
	MaitaState(MaitaState&&) noexcept = delete;
	MaitaState& operator= (const MaitaState&) = delete;
	MaitaState& operator= (MaitaState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual void NotifyPlayerObservers(PlayerComponent*) override {};
	virtual void Attack() override {};

protected:
	bool IsPlayable() const { return m_IsPlayable; };
private:
	bool m_IsPlayable;
};

#endif // !MAITASTATE_H
