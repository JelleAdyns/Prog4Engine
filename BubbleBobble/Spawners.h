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
#include "FloorCheckingComponent.h"
#include "ProjectileComponent.h"

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
		pPickUp->AddPhysicsComponent();
		pPickUp->AddComponent<SpriteComponent>("Textures/PickUps.png", 2, 3, 0.1f, false, false);
		SpriteComponent* pSpriteComp = pPickUp->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pPickUp->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::pickUp);
		pPickUp->AddComponent<FloorCheckingComponent>(glm::vec2{destRectSize.x/4, 0.f}, glm::vec2{ destRectSize.x / 2, destRectSize.y });
		pPickUp->AddComponent<PickUpComponent>(pickUpType, nullptr);

		pPickUp->Start();

		pScene->AddGameObject(std::move(pPickUp));
	}

	inline static void SpawnProjectile(const glm::vec2& spawnPos, bool left)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pBoulder = std::make_unique<dae::GameObject>(spawnPos);
		pBoulder->AddRenderComponent();
		pBoulder->AddPhysicsComponent();
		pBoulder->AddComponent<SpriteComponent>("Textures/Boulder.png", 4, 2, 0.1f, true, false);
		SpriteComponent* pSpriteComp = pBoulder->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pBoulder->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::projectileTag);
		pBoulder->AddComponent<WallCheckingComponent>(glm::vec2{ 0.f, destRectSize.y / 4 }, glm::vec2{ destRectSize.x , destRectSize.y/ 2 });
		pBoulder->AddComponent<ProjectileComponent>(left);

		pBoulder->Start();

		pScene->AddGameObject(std::move(pBoulder));
	}
}

#endif // !SPAWNERS_H
