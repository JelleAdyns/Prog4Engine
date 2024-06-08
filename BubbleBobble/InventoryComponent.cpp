#include "InventoryComponent.h"
#include "PlayerComponent.h"
#include "ScoreUIComponent.h"
#include <GameObject.h>

InventoryComponent::InventoryComponent(dae::GameObject* pOwner, ScoreUIComponent* pObserver):
	dae::Component{pOwner},
	m_pPlayerComp{},
	m_VecItems{},
	m_pAddedItem{std::make_unique<dae::Subject<InventoryComponent>>()}
{
	m_pAddedItem->AddObserver(pObserver);
}

void InventoryComponent::Start()
{
	if (!m_pPlayerComp) m_pPlayerComp = GetOwner()->GetComponent<PlayerComponent>();
}

void InventoryComponent::Update()
{
}

void InventoryComponent::PrepareImGuiRender()
{
}

void InventoryComponent::AddItem(PickUpComponent::PickUpType pickUpType, const glm::vec2& lastPos)
{
	m_VecItems.push_back(std::make_pair(pickUpType,lastPos));

	m_pAddedItem->NotifyObservers(this);
}

const std::pair<PickUpComponent::PickUpType, glm::vec2>& InventoryComponent::GetLastAddedItem() const
{
	assert((!m_VecItems.empty()) && "There were no Items in the vector");
	return m_VecItems.back();
}

PlayerComponent::PlayerType InventoryComponent::GetPlayerType() const
{
	return m_pPlayerComp->GetPlayerType();
}
