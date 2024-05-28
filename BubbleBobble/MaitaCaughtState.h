#ifndef MAITACAUGHTSTATE_H
#define MAITACAUGHTSTATE_H

#include "MaitaState.h"
#include "SpriteComponent.h"
#include <Observer.h>

namespace dae
{
	class GameObject;
	class CollisionComponent;
	class PhysicsComponent;
}

class MaitaComponent;
class FloorCheckingComponent;
class BubbleComponent;

class MaitaCaughtState final : public MaitaState, public dae::Observer<BubbleComponent>
{
public:
	explicit MaitaCaughtState(dae::GameObject* pMaita, dae::GameObject* pBubble);
	virtual ~MaitaCaughtState()
	{
		if (m_pObservedSubject) m_pObservedSubject->RemoveObserver(this);
	}

	MaitaCaughtState(const MaitaCaughtState&) = delete;
	MaitaCaughtState(MaitaCaughtState&&) noexcept = delete;
	MaitaCaughtState& operator= (const MaitaCaughtState&) = delete;
	MaitaCaughtState& operator= (MaitaCaughtState&&) noexcept = delete;

	virtual std::unique_ptr<MaitaState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Notify(BubbleComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<BubbleComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

private:

	enum class NextState
	{
		Caught,
		Popped,
		Free
	};
	NextState m_NextState{ NextState::Caught };
	static constexpr SpriteComponent::SpriteInfo m_CaughtInfo{ .rowNumber{5}, .nrOfCols{4} };

	dae::GameObject* m_pMaita;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	SpriteComponent* m_pSpriteComp;

	BubbleComponent* m_pOccupiedBubble;

	dae::Subject<BubbleComponent>* m_pObservedSubject;
};


#endif // !MAITACAUGHTSTATE_H