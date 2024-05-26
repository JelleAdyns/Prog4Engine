#ifndef ZENCHANCAUGHTSTATE_H
#define ZENCHANCAUGHTSTATE_H

#include "ZenChanState.h"
#include "SpriteComponent.h"
#include <Observer.h>

namespace dae
{
	class GameObject;
	class CollisionComponent;
	class PhysicsComponent;
}

class EnemyComponent;
class FloorCheckingComponent;
class BubbleComponent;

class ZenChanCaughtState final : public ZenChanState, public dae::Observer<BubbleComponent>
{
public:
	explicit ZenChanCaughtState(dae::GameObject* pEnemy, dae::GameObject* pBubble);
	virtual ~ZenChanCaughtState() = default;

	ZenChanCaughtState(const ZenChanCaughtState&) = delete;
	ZenChanCaughtState(ZenChanCaughtState&&) noexcept = delete;
	ZenChanCaughtState& operator= (const ZenChanCaughtState&) = delete;
	ZenChanCaughtState& operator= (ZenChanCaughtState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Notify(BubbleComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<BubbleComponent>* pSubject) override;

private:

	enum class NextState
	{
		Caught,
		Popped,
		Free
	};
	NextState m_NextState{NextState::Caught};
	static constexpr SpriteComponent::RowInfo m_CaughtInfo{ .rowNumber{3} };

	dae::GameObject* m_pEnemy;;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	SpriteComponent* m_pSpriteComp;

	dae::Subject<BubbleComponent>* m_pObservedSubject;
};


#endif // !ZENCHANCAUGHTSTATE_H