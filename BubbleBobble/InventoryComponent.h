#ifndef INVENTORYCOMPONENT_H
#define INVENTORYCOMPONENT_H

#include <Subject.h>
#include <Component.h>
#include "PickUpComponent.h"

namespace dae
{
	class GameObject;
}

class ScoreUIComponent;
class PlayerComponent;
class InventoryComponent final : public dae::Component
{
public:
	explicit InventoryComponent(dae::GameObject* pOwner, ScoreUIComponent* pObserver);
	virtual ~InventoryComponent() = default;

	InventoryComponent(const InventoryComponent&) = delete;
	InventoryComponent(InventoryComponent&&) noexcept = delete;
	InventoryComponent& operator= (const InventoryComponent&) = delete;
	InventoryComponent& operator= (InventoryComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void AddItem(PickUpComponent::PickUpType pickUpType, const glm::vec2& lastPos);

	const std::pair<PickUpComponent::PickUpType, glm::vec2>& GetLastAddedItem() const;
	PlayerComponent::PlayerType GetPlayerType() const;

private:
	
	PlayerComponent* m_pPlayerComp;

	std::vector<std::pair<PickUpComponent::PickUpType, glm::vec2>> m_VecItems;

	std::unique_ptr < dae::Subject<InventoryComponent> >m_pAddedItem;

};

#endif // !INVENTORYCOMPONENT_H