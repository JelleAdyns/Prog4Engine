#include "Spawners.h"
#include "Components.h"

#include <SceneManager.h>
#include <GameObject.h>
#include <CollisionComponent.h>
#include "CollisionTags.h"
#include "BubbleComponent.h"
#include "SpriteComponent.h"

#include "WallCheckingComponent.h"
#include "FloorCheckingComponent.h"
#include "ProjectileComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "EnemyCounterComponent.h"
#include "LevelState.h"

namespace spawners
{

	void SpawnZenChan(const glm::vec2& spawnPos, PlayerComponent* pPlayerOne, PlayerComponent* pPlayerTwo, EnemyCounterComponent* pObserver)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetNextScene();

		auto zenChan = std::make_unique<dae::GameObject>(spawnPos);
		zenChan->AddRenderComponent();
		zenChan->AddPhysicsComponent();
		zenChan->AddComponent<EnemyComponent>(EnemyComponent::EnemyType::ZenChan, pObserver);
		//zenChan->AddComponent<MovementComponent>(-160.f, 60.f);
		EnemyComponent* zenChanComp = zenChan->GetComponent<EnemyComponent>();
		zenChanComp->AddPlayerObserver(pPlayerOne);
		zenChanComp->AddPlayerObserver(pPlayerTwo);
		zenChan->AddComponent<SpriteComponent>("Textures/Zen-ChanStates.png", 4, 7, 0.1f);
		SpriteComponent* zenChanSpriteComp = zenChan->GetComponent<SpriteComponent>();

		const auto& zenChanDestRctSize = zenChanSpriteComp->GetDestRectSize();
		float collisionOffset{ LevelState::GetCollisionOffset() };

		zenChan->AddComponent<dae::CollisionComponent>(
			glm::vec2{ collisionOffset ,collisionOffset },
			glm::vec2{ zenChanDestRctSize.x - collisionOffset * 2 ,zenChanDestRctSize.y - collisionOffset * 2 },
			collisionTags::enemyTag);

		zenChan->AddComponent<WallCheckingComponent>(glm::vec2{ 0,zenChanDestRctSize.y / 4 }, glm::vec2{ zenChanDestRctSize.x,zenChanDestRctSize.y / 2 });
		zenChan->AddComponent<FloorCheckingComponent>(glm::vec2{ zenChanDestRctSize.x / 4,0 }, glm::vec2{ zenChanDestRctSize.x / 2,zenChanDestRctSize.y });

		pScene->AddGameObject(std::move(zenChan));
	}

	void SpawnMaita(const glm::vec2& spawnPos, PlayerComponent* pPlayerOne, PlayerComponent* pPlayerTwo, EnemyCounterComponent* pObserver)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetNextScene();

		auto maita = std::make_unique<dae::GameObject>(spawnPos);
		maita->AddRenderComponent();
		maita->AddPhysicsComponent();
		maita->AddComponent<EnemyComponent>(EnemyComponent::EnemyType::Maita, pObserver);
		float maitaOffset = EnemyComponent::GetMaitaOffset();
		//maita->AddComponent<MovementComponent>(-160.f, 60.f);
		EnemyComponent* maitaComp = maita->GetComponent<EnemyComponent>();
		maitaComp->AddPlayerObserver(pPlayerOne);
		maitaComp->AddPlayerObserver(pPlayerTwo);
		maita->AddComponent<SpriteComponent>("Textures/MaitaStates.png", 5, 9, 0.1f);
		SpriteComponent* maitaSpriteComp = maita->GetComponent<SpriteComponent>();

		auto maitaDestRctSize = maitaSpriteComp->GetDestRectSize();
		float collisionOffset{ LevelState::GetCollisionOffset() };

		maita->AddComponent<dae::CollisionComponent>(
			glm::vec2{ maitaOffset + collisionOffset  ,collisionOffset },
			glm::vec2{ maitaDestRctSize.x - collisionOffset * 2 - maitaOffset * 2 ,maitaDestRctSize.y - collisionOffset * 2 },
			collisionTags::enemyTag);

		maita->AddComponent<WallCheckingComponent>(glm::vec2{ maitaOffset ,maitaDestRctSize.y / 4 }, glm::vec2{ maitaDestRctSize.x - maitaOffset * 2,maitaDestRctSize.y / 2 });
		maita->AddComponent<FloorCheckingComponent>(glm::vec2{ (maitaDestRctSize.x - maitaOffset * 2) / 4 + maitaOffset, 0 }, glm::vec2{ (maitaDestRctSize.x - maitaOffset * 2) / 2,maitaDestRctSize.y });

		pScene->AddGameObject(std::move(maita));
		
	}

	void SpawnBubble(const glm::vec2& spawnPos, bool left)
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
		pBubble->AddComponent<WallCheckingComponent>(glm::vec2{ 0, destRectSize.y / 4 }, glm::vec2{ destRectSize.x, destRectSize.y / 2 });

		pBubble->Start();

		pScene->AddGameObject(std::move(pBubble));
	}

	void SpawnPickUp(const glm::vec2& spawnPos, PickUpComponent::PickUpType pickUpType)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pPickUp = std::make_unique<dae::GameObject>(spawnPos);
		pPickUp->AddRenderComponent();
		pPickUp->AddPhysicsComponent();
		pPickUp->AddComponent<PickUpComponent>(pickUpType, nullptr);
		PickUpComponent* pPickUpComp = pPickUp->GetComponent<PickUpComponent>();
		pPickUp->AddComponent<SpriteComponent>("Textures/PickUps.png", 5, 1, 0.2f, false, false);
		SpriteComponent* pSpriteComp = pPickUp->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pSpriteComp->AddObserver(pPickUpComp);
		pPickUp->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::pickUp);
		pPickUp->AddComponent<FloorCheckingComponent>(glm::vec2{ destRectSize.x / 4, 0.f }, glm::vec2{ destRectSize.x / 2, destRectSize.y });

		pPickUp->Start();

		pScene->AddGameObject(std::move(pPickUp));
	}

	void SpawnProjectile(const glm::vec2& spawnPos, bool left)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pBoulder = std::make_unique<dae::GameObject>(spawnPos);
		pBoulder->AddRenderComponent();
		pBoulder->AddPhysicsComponent();
		pBoulder->AddComponent<SpriteComponent>("Textures/Boulder.png", 4, 2, 0.1f, true, false);
		SpriteComponent* pSpriteComp = pBoulder->GetComponent<SpriteComponent>();
		auto destRectSize = pSpriteComp->GetDestRectSize();
		pBoulder->AddComponent<dae::CollisionComponent>(glm::vec2{}, destRectSize, collisionTags::projectileTag);
		pBoulder->AddComponent<WallCheckingComponent>(glm::vec2{ 0.f, destRectSize.y / 4 }, glm::vec2{ destRectSize.x , destRectSize.y / 2 });
		pBoulder->AddComponent<ProjectileComponent>(left);

		pBoulder->Start();

		pScene->AddGameObject(std::move(pBoulder));
	}

	void SpawnFloatingScore(const glm::vec2& spawnPos, PickUpComponent::PickUpType pickUpType, PlayerComponent::PlayerType playerType)
	{
		dae::Scene* pScene = dae::SceneManager::GetInstance().GetActiveScene();

		auto pScore = std::make_unique<dae::GameObject>(spawnPos);
		pScore->AddRenderComponent();
		pScore->AddComponent<ScoreComponent>(pickUpType, playerType);
		pScore->AddComponent<SpriteComponent>("Textures/Scores.png", 2, 2, 0.1f, false, false);

		pScore->Start();

		pScene->AddGameObject(std::move(pScore));
	}
}