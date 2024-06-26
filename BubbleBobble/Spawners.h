#ifndef SPAWNERS_H
#define SPAWNERS_H

#include "PickUpComponent.h"
#include "PlayerComponent.h"

class EnemyCounterComponent;
namespace spawners
{
	void SpawnZenChan(const glm::vec2& spawnPos, PlayerComponent* pPlayerOne, PlayerComponent* pPlayerTwo, EnemyCounterComponent* pObserver);

	void SpawnMaita(const glm::vec2& spawnPos, PlayerComponent* pPlayerOne, PlayerComponent* pPlayerTwo, EnemyCounterComponent* pObserver);

	void SpawnBubble(const glm::vec2& spawnPos, PlayerComponent::PlayerType shooter, bool left);

	void SpawnPickUp(const glm::vec2& spawnPos, PickUpComponent::PickUpType pickUpType);

	void SpawnProjectile(const glm::vec2& spawnPos, bool left);

	void SpawnFloatingScore(const glm::vec2& spawnPos, int score, PlayerComponent::PlayerType playerType);
}

#endif // !SPAWNERS_H
