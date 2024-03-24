#ifndef PICKUPCOMPONENT_H
#define PICKUPCOMPONENT_H

#include "Component.h"
#include "Subject.h"
namespace dae
{
	class GameObject;
	class PickUpComponent final : public Component, public Subject<PickUpComponent>
	{
	public:
		explicit PickUpComponent(GameObject* pOwner);
		virtual ~PickUpComponent() = default;

		PickUpComponent(const PickUpComponent&) = delete;
		PickUpComponent(PickUpComponent&&) noexcept = delete;
		PickUpComponent& operator= (const PickUpComponent&) = delete;
		PickUpComponent& operator= (PickUpComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;
		void PickUp(int scoreValue);
		int GetScoreValue() const;
	private:
		int m_ScoreValue;
	};
}

#endif // !PICKUPCOMPONENT_H