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
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	
	//virtual void InflictDamage() = 0;
	virtual void Shoot() = 0;
	virtual void StopShooting() = 0;

protected:
	static int GetShootStartIndex() { return m_ShootStartIndex; }

private:
	static const int m_ShootStartIndex{ 4 };
};


#endif // !PLAYERSTATE_H
