#ifndef ZENCHANCAUGHTSTATE_H
#define ZENCHANCAUGHTSTATE_H

#include "ZenChanState.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include "FloorCheckingComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>
#include <Observer.h>


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

	bool m_Pop{ false };
	static constexpr SpriteComponent::RowInfo m_CaughtInfo{ .rowNumber{3} };

	dae::GameObject* m_pEnemy;;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	SpriteComponent* m_pSpriteComp;

	dae::Subject<BubbleComponent>* m_pObservedSubject;
};


#endif // !ZENCHANCAUGHTSTATE_H