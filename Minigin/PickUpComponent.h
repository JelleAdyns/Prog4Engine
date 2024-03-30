#ifndef PICKUPCOMPONENT_H
#define PICKUPCOMPONENT_H

#include "Component.h"
#include "Subject.h"
namespace dae
{
	class GameObject;
	class ScoreUIComponent;
	class PickUpComponent final : public Component
	{
	public:
		enum class PickUpType
		{
			Melon,
			Fries
		};
		explicit PickUpComponent(GameObject* pOwner, PickUpComponent::PickUpType pickUpType, ScoreUIComponent* pObserver);
		virtual ~PickUpComponent() = default;

		PickUpComponent(const PickUpComponent&) = delete;
		PickUpComponent(PickUpComponent&&) noexcept = delete;
		PickUpComponent& operator= (const PickUpComponent&) = delete;
		PickUpComponent& operator= (PickUpComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;
		void PickUp();
		PickUpType GetPickUpType() const;
	private:
		PickUpType m_PickUpType;
		std::unique_ptr<Subject<PickUpComponent>> m_PickedUp;
	};
}

#endif // !PICKUPCOMPONENT_H