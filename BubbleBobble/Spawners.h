#ifndef SPAWNERS_H
#define SPAWNERS_H

#include "PickUpComponent.h"

class PlayerComponent;
namespace spawners
{
	void SpawnZenChan(const glm::vec2& spawnPos, PlayerComponent* pPlayerOne, PlayerComponent* pPlayerTwo);
	
	void SpawnMaita(const glm::vec2& spawnPos, PlayerComponent* pPlayerOne, PlayerComponent* pPlayerTwo);

	void SpawnBubble(const glm::vec2& spawnPos, bool left);

	void SpawnPickUp(const glm::vec2& spawnPos, PickUpComponent::PickUpType pickUpType);

	void SpawnProjectile(const glm::vec2& spawnPos, bool left);
}

#endif // !SPAWNERS_H
