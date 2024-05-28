#ifndef MAITARUNSTATE_H
#define MAITARUNSTATE_H

#include "MaitaState.h"
#include "PlayerComponent.h"
#include "SpriteComponent.h"
#include <Observer.h>
namespace dae
{
	class GameObject;
	class CollisionComponent;
	class PhysicsComponent;
}

class MaitaComponent;
class WallCheckingComponent;
class FloorCheckingComponent;
class MaitaRunState final : public MaitaState, public dae::Observer<PlayerComponent>
{
public:
	explicit MaitaRunState(dae::GameObject* pMaita, MaitaComponent* pMaitaComp, bool isAngry = false);
	virtual ~MaitaRunState()
	{
		for (dae::Subject<PlayerComponent>* pSpriteSubject : m_pVecObservedSpriteSubjects)
		{
			if (pSpriteSubject) pSpriteSubject->RemoveObserver(this);
		}
	}

	MaitaRunState(const MaitaRunState&) = delete;
	MaitaRunState(MaitaRunState&&) noexcept = delete;
	MaitaRunState& operator= (const MaitaRunState&) = delete;
	MaitaRunState& operator= (MaitaRunState&&) noexcept = delete;

	virtual std::unique_ptr<MaitaState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Notify(PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

private:


	static constexpr SpriteComponent::SpriteInfo m_RunInfo{ .rowUpdate{false}, .rowNumber{0}, .nrOfCols{5}, .frameTime{0.1f} };
	static constexpr float m_GeneralSpeed{ 50.f };
	const float m_Speed;

	float m_AttackDelayTimer{};
	const float m_AttackDelay{ 0.7f };

	bool m_HasToJump{ false };
	bool m_HasToAttack{ false };
	bool m_IsAngry;

	dae::GameObject* m_pMaita;
	MaitaComponent* m_pMaitaComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
	WallCheckingComponent* m_pWallCheckingComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
	dae::CollisionComponent* m_pCollisionComp;

	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSpriteSubjects;
};


#endif // !MAITARUNSTATE_H