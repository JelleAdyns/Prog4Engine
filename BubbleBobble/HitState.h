#ifndef HITSTATE_H
#define HITSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "MovementComponent.h"
#include "Commands.h"
#include <CollisionComponent.h>
#include <KeyState.h>
#include <Observer.h>
#include <GameObject.h>


class HitState final : public PlayerState, public dae::Observer<SpriteComponent>
{
public:
	explicit HitState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp) :
		PlayerState{},
		m_pPlayer{ pPlayer },
		m_pPlayerComp{ pPlayerComp },
		m_pMovementComp{ pMovementComp },
		m_pSpriteComp{ pPlayer->GetComponent<SpriteComponent>()}
	{}
	virtual ~HitState() = default;

	HitState(const HitState&) = delete;
	HitState(HitState&&) noexcept = delete;
	HitState& operator= (const HitState&) = delete;
	HitState& operator= (HitState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Shoot() override;

	virtual void Notify(SpriteComponent* pSpriteComp) override;
	virtual void AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject) override;


	static float GetHitSpriteOffset();
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	MovementComponent* m_pMovementComp;
	SpriteComponent* m_pSpriteComp;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSpriteSubjects;

	const int m_NrOfRows{ 3 };
	int m_RowCount{ };
	static const float m_HitSpriteOffset;
};


#endif // !HITSTATE_H