#ifndef MAITASTATE_H
#define MAITASTATE_H

#include <memory>

class MaitaState
{
public:
	MaitaState() = default;
	virtual ~MaitaState() = default;

	MaitaState(const MaitaState&) = delete;
	MaitaState(MaitaState&&) noexcept = delete;
	MaitaState& operator= (const MaitaState&) = delete;
	MaitaState& operator= (MaitaState&&) noexcept = delete;

	virtual std::unique_ptr<MaitaState> Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

};

#endif // !MAITASTATE_H
