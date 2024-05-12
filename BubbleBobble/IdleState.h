#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"

#include "Commands.h"
#include <PhysicsComponent.h>
#include <KeyState.h>
#include <GameObject.h>
#include <InputCommandBinder.h>

class IdleState final : public PlayerState
{
public:
	explicit IdleState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp },
		m_pPhysicsComp{ pPlayer->GetComponent<dae::PhysicsComponent>() }
	{}
	virtual ~IdleState() = default;

	IdleState(const IdleState&) = delete;
	IdleState(IdleState&&) noexcept = delete;
	IdleState& operator= (const IdleState&) = delete;
	IdleState& operator= (IdleState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override;
	virtual void OnEnter() const override;
	virtual void OnExit() const override;

	static float GetNormalSpriteEndheight();
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	dae::PhysicsComponent* m_pPhysicsComp;

	static const float m_NormalSpritesEndHeight;
};


#endif // !IDLESTATE_H
