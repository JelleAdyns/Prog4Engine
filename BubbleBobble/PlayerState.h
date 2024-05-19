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

	virtual std::unique_ptr<PlayerState> Update()
	{
		/*if (m_IsInvincible)
		{
			m_InvincibilityTimer += dae::GameTime::GetInstance().GetDeltaTime();

			if (m_InvincibilityTimer >= m_InvincibilityMaxTime)
			{
				m_InvincibilityTimer = 0.f;
				m_IsInvincible = false;
			}
		}*/
		return nullptr;
	};
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	
	virtual void Shoot() = 0;

};


#endif // !PLAYERSTATE_H
