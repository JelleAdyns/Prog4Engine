#ifndef SPAWNERS_H
#define SPAWNERS_H

#include <SceneManager.h>
#include <GameObject.h>
#include <CollisionComponent.h>
#include "CollisionTags.h"
#include "BubbleComponent.h"
#include "SpriteComponent.h"
#include "PickUpComponent.h"
#include "WallCheckingComponent.h"

namespace spawners
{
	inline static void SpawnBubble(const glm::vec2& spawnPos, bool left)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pBubble = std::make_unique<dae::GameObject>(spawnPos);
		pBubble->AddRenderComponent();
		pBubble->AddPhysicsComponent();
		pBubble->AddComponent<BubbleComponent>(left);
		pBubble->AddComponent<SpriteComponent>("Textures/Bubble.png", 3, 11, 0.1f, true, true);
		SpriteComponent* pSpriteComp = pBubble->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pBubble->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::bubbleTag);
		pBubble->AddComponent<WallCheckingComponent>(glm::vec2{ 0, destRectSize.y / 4 }, glm::vec2{ destRectSize.x, destRectSize.y /2});

		pBubble->Start();

		pScene->AddGameObject(std::move(pBubble));
	}

	inline static void SpawnPickUp(const glm::vec2& spawnPos, PickUpComponent::PickUpType pickUpType)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pPickUp = std::make_unique<dae::GameObject>(spawnPos);
		pPickUp->AddRenderComponent();
		pPickUp->AddComponent<SpriteComponent>("Textures/PickUps.png", 2, 3, 0.1f, false, false);
		SpriteComponent* pSpriteComp = pPickUp->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pPickUp->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::pickUp);
		pPickUp->AddComponent<PickUpComponent>(pickUpType, nullptr);

		pPickUp->Start();

		pScene->AddGameObject(std::move(pPickUp));
	}
}

#endif // !SPAWNERS_H
