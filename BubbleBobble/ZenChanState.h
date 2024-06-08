#ifndef ZENCHANSTATE_H
#define ZENCHANSTATE_H

#include <memory>
#include "EnemyState.h"

class PlayerComponent;
class ZenChanState: public EnemyState
{
public:
	ZenChanState():
		EnemyState{}
	{};
	virtual ~ZenChanState() = default;

	ZenChanState(const ZenChanState&) = delete;
	ZenChanState(ZenChanState&&) noexcept = delete;
	ZenChanState& operator= (const ZenChanState&) = delete;
	ZenChanState& operator= (ZenChanState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual void NotifyPlayerObservers(PlayerComponent*) override = 0;
	virtual void Attack() override {};
};

#endif // !ZENCHANSTATE_H
