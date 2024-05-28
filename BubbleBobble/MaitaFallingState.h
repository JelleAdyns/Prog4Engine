#ifndef MAITAFALLINGSTATE_H
#define MAITAFALLINGSTATE_H

#include "MaitaState.h"
#include "PlayerComponent.h"
#include <Observer.h>

namespace dae
{
	class GameObject;
	class PhysicsComponent;
	class CollisionComponent;
}

class MaitaComponent;
class SpriteComponent;
class FloorCheckingComponent;
class MaitaFallingState final : public MaitaState, public dae::Observer<PlayerComponent>
{
public:
	explicit MaitaFallingState(dae::GameObject* pMaita, MaitaComponent* pMaitaComp, bool isAngry);
	virtual ~MaitaFallingState()
	{
		for (dae::Subject<PlayerComponent>* pSpriteSubject : m_pVecObservedSubjects)
		{
			if (pSpriteSubject) pSpriteSubject->RemoveObserver(this);
		}
	}

	MaitaFallingState(const MaitaFallingState&) = delete;
	MaitaFallingState(MaitaFallingState&&) noexcept = delete;
	MaitaFallingState& operator= (const MaitaFallingState&) = delete;
	MaitaFallingState& operator= (MaitaFallingState&&) noexcept = delete;

	virtual std::unique_ptr<MaitaState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Notify(PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

private:

	bool m_IsAngry;
	float m_PlayerXPos{};

	dae::GameObject* m_pMaita;
	MaitaComponent* m_pMaitaComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	FloorCheckingComponent* m_pFloorCheckingComp;


	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSubjects;
};


#endif // !MAITAFALLINGSTATE_H