#ifndef PICKUPCOMMAND_H
#define PICKUPCOMMAND_H

#include "GameObjectCommand.h"
#include "PickUpComponent.h"
#include "GameTime.h"

namespace dae
{
	class PickUpCommand final : public GameObjectCommand
	{
	public:
		PickUpCommand(const std::unique_ptr<GameObject>& pObject) :
			GameObjectCommand{ pObject.get() },
			m_pPickUpComponent{pObject->GetComponent<PickUpComponent>()}
		{}
		virtual ~PickUpCommand() = default;

		PickUpCommand(const PickUpCommand&) = delete;
		PickUpCommand(PickUpCommand&&) noexcept = delete;
		PickUpCommand& operator= (const PickUpCommand&) = delete;
		PickUpCommand& operator= (PickUpCommand&&) noexcept = delete;
		virtual void Execute() const override
		{
			m_pPickUpComponent->PickUp();
		}
	private:
		PickUpComponent* m_pPickUpComponent;
	};
}
#endif // !PICKUPCOMMAND_H