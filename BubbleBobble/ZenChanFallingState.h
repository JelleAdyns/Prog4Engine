#ifndef ZENCHANFALLINGSTATE_H
#define ZENCHANFALLINGSTATE_H

#include "ZenChanState.h"
#include "PlayerComponent.h"
#include <Observer.h>

namespace dae
{
	class GameObject;
	class PhysicsComponent;
	class CollisionComponent;
}

class ZenChanComponent;
class SpriteComponent;
class FloorCheckingComponent;
class ZenChanFallingState final : public ZenChanState, public dae::Observer<PlayerComponent>
{
public:
	explicit ZenChanFallingState(dae::GameObject* pZenChan, ZenChanComponent* pZenChanComp, bool isAngry);
	virtual ~ZenChanFallingState()
	{
		for (dae::Subject<PlayerComponent>* pSpriteSubject : m_pVecObservedSubjects)
		{
			if (pSpriteSubject) pSpriteSubject->RemoveObserver(this);
		}
	}

	ZenChanFallingState(const ZenChanFallingState&) = delete;
	ZenChanFallingState(ZenChanFallingState&&) noexcept = delete;
	ZenChanFallingState& operator= (const ZenChanFallingState&) = delete;
	ZenChanFallingState& operator= (ZenChanFallingState&&) noexcept = delete;

	virtual std::unique_ptr<ZenChanState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Notify(PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

private:

	bool m_IsAngry;
	float m_PlayerXPos{};

	dae::GameObject* m_pZenChan;
	ZenChanComponent* m_pZenChanComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	FloorCheckingComponent* m_pFloorCheckingComp;


	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSubjects;
};


#endif // !ZENCHANFALLINGSTATE_H