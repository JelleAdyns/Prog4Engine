#ifndef HITSTATE_H
#define HITSTATE_H

#include "PlayerState.h"
#include "SpriteComponent.h"
#include <Observer.h>

namespace dae
{
	class GameObject;
}

class PlayerComponent;
class MovementComponent;
class HitState final : public PlayerState, public dae::Observer<SpriteComponent>
{
public:
	explicit HitState(dae::GameObject* pPlayer, PlayerComponent* pPlayerComp, MovementComponent* pMovementComp);
	virtual ~HitState()
	{
		for (dae::Subject<SpriteComponent>* pSpriteSubject : m_pVecObservedSpriteSubjects)
		{
			if(pSpriteSubject) pSpriteSubject->RemoveObserver(this);
		}
	}

	HitState(const HitState&) = delete;
	HitState(HitState&&) noexcept = delete;
	HitState& operator= (const HitState&) = delete;
	HitState& operator= (HitState&&) noexcept = delete;

	virtual std::unique_ptr<PlayerState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Shoot() override;
	virtual void StopShooting() override;

	virtual void Notify(SpriteComponent* pSpriteComp) override;
	virtual void AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid(dae::Subject<SpriteComponent>* pSubject) override;


	static float GetHitSpriteOffset();
private:
	dae::GameObject* m_pPlayer;
	PlayerComponent* m_pPlayerComp;
	MovementComponent* m_pMovementComp;
	SpriteComponent* m_pSpriteComp;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSpriteSubjects;

	int m_RowCount{ };
	static const float m_HitSpriteOffset;
	static constexpr SpriteComponent::SpriteInfo m_HitSpriteInfo{ .rowNumber = 0, .nrOfRows = 3, .frameTime{0.2f} };
};


#endif // !HITSTATE_H