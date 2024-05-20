#ifndef BUBBLECOMPONENT_H
#define BUBBLECOMPONENT_H

#include <vector>
#include <Component.h>
#include <Observer.h>

class EnemyComponent;
class BubbleComponent final : public dae::Component, public dae::Observer<EnemyComponent>
{
public:
	explicit BubbleComponent(dae::GameObject* pOwner);
	virtual ~BubbleComponent() = default;

	BubbleComponent(const BubbleComponent&) = delete;
	BubbleComponent(BubbleComponent&&) noexcept = delete;
	BubbleComponent& operator= (const BubbleComponent&) = delete;
	BubbleComponent& operator= (BubbleComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(EnemyComponent* pSpriteComp) override;
	virtual void AddSubjectPointer(dae::Subject<EnemyComponent>* pSubject) override;
private:

	std::vector<dae::Subject<EnemyComponent>*> m_pVecObservedSpriteSubjects;

};
#endif // !BUBBLECOMPONENT_H