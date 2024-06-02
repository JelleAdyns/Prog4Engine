#ifndef ENEMYSTATE_H
#define ENEMYSTATE_H

#include <memory>

class EnemyState
{
public:
	EnemyState() = default;
	virtual ~EnemyState() = default;

	EnemyState(const EnemyState&) = delete;
	EnemyState(EnemyState&&) noexcept = delete;
	EnemyState& operator= (const EnemyState&) = delete;
	EnemyState& operator= (EnemyState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

};

#endif // !ENEMYSTATE_H
