#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include <memory>

class PlayerState
{
public:
	PlayerState() = default;
	virtual ~PlayerState() = default;

	PlayerState(const PlayerState&) = delete;
	PlayerState(PlayerState&&) noexcept = delete;
	PlayerState& operator= (const PlayerState&) = delete;
	PlayerState& operator= (PlayerState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() = 0;
	virtual void OnEnter() const = 0;
	virtual void OnExit() const = 0;

};


#endif // !PLAYERSTATE_H
