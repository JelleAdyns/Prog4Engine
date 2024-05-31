#ifndef SPAWNERS_H
#define SPAWNERS_H

#include "PickUpComponent.h"

namespace spawners
{
	void SpawnEnemy(const glm::vec2& spawnPos);

	void SpawnBubble(const glm::vec2& spawnPos, bool left);

	void SpawnPickUp(const glm::vec2& spawnPos, PickUpComponent::PickUpType pickUpType);

	void SpawnProjectile(const glm::vec2& spawnPos, bool left);
}

#endif // !SPAWNERS_H
