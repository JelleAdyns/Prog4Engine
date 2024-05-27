#ifndef ZENCHANSTATE_H
#define ZENCHANSTATE_H

#include <memory>

class ZenChanState
{
public:
	ZenChanState() = default;
	virtual ~ZenChanState() = default;

	ZenChanState(const ZenChanState&) = delete;
	ZenChanState(ZenChanState&&) noexcept = delete;
	ZenChanState& operator= (const ZenChanState&) = delete;
	ZenChanState& operator= (ZenChanState&&) noexcept = delete;

	virtual std::unique_ptr<ZenChanState> Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

};

#endif // !ZENCHANSTATE_H
