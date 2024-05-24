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
class WallCheckingComponent;
class BubbleComponent final : public dae::Component, public dae::Observer<SpriteComponent>
{
public:
	explicit BubbleComponent(dae::GameObject* pOwner, bool left);
	virtual ~BubbleComponent();

	BubbleComponent(const BubbleComponent&) = delete;
	BubbleComponent(BubbleComponent&&) noexcept = delete;
	BubbleComponent& operator= (const BubbleComponent&) = delete;
	BubbleComponent& operator= (BubbleComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;


	virtual void Notify(SpriteComponent* pSpriteComp) override;
	virtual void AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject) override;
private:
	enum class BubbleState
	{
		Shot,
		Floating,
		Popped
	};

	static constexpr int m_MaxHeight{ 50 };
	static constexpr float m_XVelocity{ 120.f };

	bool m_Left{};
	int m_RowCount{};

	float m_TimeBeforePop{};
	float m_TimeToPop{10.f};
	BubbleState m_CurrState;
	SpriteComponent* m_pSpriteComp;
	WallCheckingComponent* m_pWallComp;
	dae::PhysicsComponent* m_pPhysicsComp;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSpriteSubjects;

};
#endif // !BUBBLECOMPONENT_H