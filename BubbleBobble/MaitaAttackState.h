#ifndef MAITAATTACKSTATE_H
#define MAITAATTACKSTATE_H

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
class MaitaAttackState final : public MaitaState, public dae::Observer<SpriteComponent>
{
public:
	explicit MaitaAttackState(dae::GameObject* pMaita, MaitaComponent* pMaitaComp, bool isAngry = false);
	virtual ~MaitaAttackState();

	MaitaAttackState(const MaitaAttackState&) = delete;
	MaitaAttackState(MaitaAttackState&&) noexcept = delete;
	MaitaAttackState& operator= (const MaitaAttackState&) = delete;
	MaitaAttackState& operator= (MaitaAttackState&&) noexcept = delete;

	virtual std::unique_ptr<MaitaState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Notify(SpriteComponent* pSpriteComp) override;
	virtual void AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;
private:
	static constexpr SpriteComponent::SpriteInfo m_AttackInfo{ .rowUpdate{false}, .rowNumber{2}, .nrOfCols{5}, .frameTime{0.2f}};

	const bool m_IsAngry;
	bool m_Done{ false };

	dae::GameObject* m_pMaita;
	MaitaComponent* m_pMaitaComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	SpriteComponent* m_pSpriteComp;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSpriteSubjects;
};


#endif // !MAITAATTACKSTATE_H