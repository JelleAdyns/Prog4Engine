#ifndef BUBBLECOMPONENT_H
#define BUBBLECOMPONENT_H

#include <vector>
#include <Component.h>
#include <Subject.h>


namespace dae
{
	class PhysicsComponent;
	class CollisionComponent;
	class RenderComponent;
}

class SpriteComponent;
class WallCheckingComponent;
class BubbleComponent final : public dae::Component, public dae::Observer<SpriteComponent>
{
public:
	enum class FloatingStage
	{
		ShooterColor,
		Pink,
		Red,
	};


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
	virtual void SetSubjectPointersInvalid() override;

	void AddObserver(dae::Observer<BubbleComponent>* pObserver);
	void SetOccupied();
	bool IsOccupied();
	bool IsPoppedByPlayer();
	FloatingStage GetFloatingStage() const;
	bool IsFloating() const;

private:
	enum class BubbleState
	{
		Shot,
		Floating,
		Popped
	};

	void HandleShotState();
	void HandleFloatingState();
	void HandlePoppedState();

	static constexpr int m_MaxHeight{ 30 };
	static constexpr float m_XSpeed{ 120.f };
	static constexpr float m_YVelocity{ -20.f };

	bool m_PoppedByPlayer{ false };
	bool m_IsOccupied{ false };
	bool m_Left{};
	int m_RowCount{};

	FloatingStage m_FloatingStage{FloatingStage::ShooterColor};
	float m_TimeBeforePop{};
	float m_TimeToPop{7.f};
	BubbleState m_CurrState;
	SpriteComponent* m_pSpriteComp;
	WallCheckingComponent* m_pWallComp;
	dae::CollisionComponent* m_pCollisionComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::RenderComponent* m_pRenderComp;
	
	std::unique_ptr<dae::Subject<BubbleComponent>> m_pTimerTick;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSpriteSubjects;

};
#endif // !BUBBLECOMPONENT_H