#ifndef PICKUPCOMPONENT_H
#define PICKUPCOMPONENT_H

#include <Component.h>
#include <Observer.h>
#include "SpriteComponent.h"
#include "PlayerComponent.h"

namespace dae
{
	class GameObject;
	class CollisionComponent;
	class RenderComponent;
}

class ScoreUIComponent;
class PickUpComponent final : public dae::Component, public dae::Observer<SpriteComponent>
{
public:
	enum class PickUpType
	{
		Melon,
		Fries
	};
	explicit PickUpComponent(dae::GameObject* pOwner, PickUpComponent::PickUpType pickUpType, ScoreUIComponent* pObserver);
	virtual ~PickUpComponent();

	PickUpComponent(const PickUpComponent&) = delete;
	PickUpComponent(PickUpComponent&&) noexcept = delete;
	PickUpComponent& operator= (const PickUpComponent&) = delete;
	PickUpComponent& operator= (PickUpComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(SpriteComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

	void PickUp(PlayerComponent::PlayerType playerType);

	PickUpType GetPickUpType() const;
private:
	void HandleTimers();

	PickUpType m_PickUpType;

	float m_Timer{};
	static constexpr float m_MaxTimeAlive{ 10.f };
	static constexpr float m_TimeToFlicker{ m_MaxTimeAlive - 3.f };

	float m_FlickerTimer{};
	static constexpr float m_FlickerDelay{ 0.2f };

	dae::CollisionComponent* m_pCollisionComp;
	dae::RenderComponent* m_pRenderComp;
	SpriteComponent* m_pSpriteComp;

	std::unique_ptr<dae::Subject<PickUpComponent>> m_PickedUp;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSubjects;
};


#endif // !PICKUPCOMPONENT_H