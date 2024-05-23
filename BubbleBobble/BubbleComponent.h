#ifndef BUBBLECOMPONENT_H
#define BUBBLECOMPONENT_H

#include <vector>
#include <Component.h>
#include <Observer.h>

namespace dae
{
	class PhysicsComponent;
}

class EnemyComponent;
class SpriteComponent;
class BubbleComponent final : public dae::Component, public dae::Observer<EnemyComponent>, public dae::Observer<SpriteComponent>
{
public:
	explicit BubbleComponent(dae::GameObject* pOwner);
	virtual ~BubbleComponent();

	BubbleComponent(const BubbleComponent&) = delete;
	BubbleComponent(BubbleComponent&&) noexcept = delete;
	BubbleComponent& operator= (const BubbleComponent&) = delete;
	BubbleComponent& operator= (BubbleComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(EnemyComponent* pSpriteComp) override;
	virtual void AddSubjectPointer(dae::Subject<EnemyComponent>* pSubject) override;

	virtual void Notify(SpriteComponent* pSpriteComp) override;
	virtual void AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject) override;
private:
	enum class BubbleState
	{
		Shot,
		Floating,
		Popped
	};

	float m_TimeBeforePop{};
	float m_TimeToPop{5.f};
	BubbleState m_CurrState;
	SpriteComponent* m_pSpriteComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	std::vector<dae::Subject<EnemyComponent>*> m_pVecObservedEnemySubjects;
	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSpriteSubjects;

};
#endif // !BUBBLECOMPONENT_H