#ifndef HITSTATE_H
#define HITSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "Commands.h"
#include <CollisionComponent.h>
#include <KeyState.h>
#include <GameObject.h>
#include <InputCommandBinder.h>

class HitState final : public PlayerState
{
public:
	explicit HitState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp },
		m_pSpriteComp{ pPlayer->GetComponent<SpriteComponent>()}
	{}
	virtual ~HitState() = default;

	HitState(const HitState&) = delete;
	HitState(HitState&&) noexcept = delete;
	HitState& operator= (const HitState&) = delete;
	HitState& operator= (HitState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override;
	virtual void OnEnter() const override;
	virtual void OnExit() const override;

private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	SpriteComponent* m_pSpriteComp;

	const int m_NrOfRows{ 3 };
	int m_RowCount{ };
	static const float m_HitSpriteOffset;
};


#endif // !HITSTATE_H