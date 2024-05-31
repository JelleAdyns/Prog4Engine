#include "LevelState.h"
#include "Components.h"
#include "NextLevelCommand.h"
#include <SceneManager.h>
#include <ResourceManager.h>
#include <Scene.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <GameObject.h>
#include <Minigin.h>
#include <filesystem>
#include "CollisionTags.h"

const std::string LevelState::m_SceneName{ "Level" };

void LevelState::OnEnter()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName + std::to_string(m_LevelNumber));

	auto pPlayer = std::make_unique<dae::GameObject>();
	MakePlayer(pPlayer); 

	LoadLevel("Levels.txt");

	scene.AddGameObject(std::move(pPlayer));
	
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	std::shared_ptr<dae::Command> pNextLevelCmd = std::make_shared<NextLevelCommand>(this);
	inputMan.AddKeyCommand(pNextLevelCmd, SDL_SCANCODE_F1, dae::KeyState::DownThisFrame);

	auto& ss2 = dae::AudioLocator::GetAudioService();
	ss2.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::MainTheme), 80, true);
}

void LevelState::OnExit()
{
}

void LevelState::OnSuspend()
{
}

void LevelState::OnResume()
{
}

void LevelState::AdvanceLevel()
{
	if(m_LevelNumber == m_MaxLevel)
	{

	}
	else
	{
		auto& sceneMan = dae::SceneManager::GetInstance();


		++m_LevelNumber;
		dae::Scene* pScene = sceneMan.GetActiveScene();
		pScene->RemoveAll();

		auto pPlayer = std::make_unique<dae::GameObject>();
		MakePlayer(pPlayer);

		LoadLevel(m_LevelFile);
		pScene->AddGameObject(std::move(pPlayer));

		pScene->Start();

	}

	
}

void LevelState::MakePlayer(const std::unique_ptr<dae::GameObject>& pPlayer)
{

	pPlayer->AddRenderComponent();
	pPlayer->AddPhysicsComponent();
	pPlayer->AddComponent<MovementComponent>(-160.f, 60.f);
	dae::PhysicsComponent::SetGravity(300);
	pPlayer->AddComponent<PlayerComponent>();
	PlayerComponent* playerComp = pPlayer->GetComponent<PlayerComponent>();
	pPlayer->AddComponent<SpriteComponent>("Textures/BubStates.png", 4, 8, 0.1f, true, false);
	SpriteComponent* spriteComp = pPlayer->GetComponent<SpriteComponent>();
	spriteComp->AddObserver(playerComp);
	spriteComp->SetHeightMarkers(0, HitState::GetHitSpriteOffset());

	const auto& destRctSize = spriteComp->GetDestRectSize();

	pPlayer->AddComponent<dae::CollisionComponent>(
		glm::vec2{ m_GeneralCollisionOffset ,m_GeneralCollisionOffset },
		glm::vec2{ destRctSize.x - m_GeneralCollisionOffset * 2 ,destRctSize.y - m_GeneralCollisionOffset * 2 },
		collisionTags::playerTag);

	pPlayer->AddComponent<WallCheckingComponent>(glm::vec2{ 0,destRctSize.y / 4 }, glm::vec2{ destRctSize.x,destRctSize.y / 2 });
	pPlayer->AddComponent<FloorCheckingComponent>(glm::vec2{ destRctSize.x / 4,0 }, glm::vec2{ destRctSize.x / 2,destRctSize.y });

	//MovementComponent* movementComp = pPlayer->GetComponent<MovementComponent>();
//if(movementComp->GetPlayerIndex() == 0)
	 m_pPlayerOne = pPlayer.get();
	//else m_pPlayerTwo = pPlayer.get();

}
void LevelState::LoadLevel(const std::string& filename)
{
	std::string path{ dae::ResourceManager::GetInstance().GetDataPath() + filename };

	if (std::ifstream inputFile{ path, std::ios::in }; inputFile.is_open())
	{
		std::string level{};

		std::stringstream infoStream{};
		ParseLevelInfo(m_LevelNumber, inputFile, infoStream);

		ParseStage(m_LevelNumber, infoStream);

		ParseEnemies(infoStream);

	}
	else { std::cout << "Couldn't open file: " << filename << '\n'; }
}

void LevelState::ParseLevelInfo(int levelNumber, std::ifstream& file, std::stringstream& levelInfoStream)
{

	std::string levelInfo{};

	while (std::getline(file, levelInfo, '-'))
	{
		if (levelInfo.find("Level " + std::to_string(levelNumber)) != std::string::npos)
		{
			levelInfo.erase(0, levelInfo.find("01"));
			levelInfoStream << levelInfo;
			break;
		}
	}


}

void LevelState::ParseStage(int levelNumber, std::stringstream& levelInfoStream)
{
	std::string level{};

	std::getline(levelInfoStream, level, ':');

	std::stringstream levelStream{ level };
	std::string line{};

	dae::Scene* activeScene = dae::SceneManager::GetInstance().GetNextScene();

	int row{};
	while (std::getline(levelStream, line))
	{
		if (line.size() >= 2) line.erase(0, 2);
		std::stringstream sStream{ line };

		int col{};

		std::for_each(line.cbegin(), line.cend(), [&](char character)
			{
				auto spawnTile = [col, row, levelNumber, &activeScene](const std::string& collisionTag)
					{
						auto pTile = std::make_unique<dae::GameObject>();
						pTile->AddRenderComponent();
						pTile->AddComponent<SpriteComponent>("Textures/Tiles.png", 3, 1, 0.1f, false);

						auto destRect = pTile->GetComponent<SpriteComponent>()->GetDestRectSize();
						pTile->SetLocalPos(static_cast<float>(destRect.x) * col, static_cast<float>(destRect.x) * row);

						if (collisionTag.size() > 0) pTile->AddComponent<dae::CollisionComponent>(glm::vec2{ }, glm::vec2{ destRect.x,destRect.y }, collisionTag);

						pTile->GetComponent<SpriteComponent>()->SetCol(levelNumber - 1);
						activeScene->AddGameObject(std::move(pTile));
					};

				switch (character)
				{
				case 'p':
					spawnTile(collisionTags::platformTag);
					break;
				case 'w':
					spawnTile(collisionTags::wallTag);
					break;
				case 'f':
					spawnTile("");
					break;
				}
				++col;
			});

		++row;
	}

	float extraHeightToBeSafe{ 100.f };
	float wallThickness{ 16.f };

	auto tile = std::make_unique<dae::GameObject>(0.f, -extraHeightToBeSafe);
	tile->AddComponent<dae::CollisionComponent>(glm::vec2{ }, glm::vec2{ wallThickness,extraHeightToBeSafe + wallThickness }, collisionTags::wallTag);
	activeScene->AddGameObject(std::move(tile));

	tile = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x - wallThickness, -extraHeightToBeSafe);
	tile->AddComponent<dae::CollisionComponent>(glm::vec2{ }, glm::vec2{ wallThickness, extraHeightToBeSafe + wallThickness }, collisionTags::wallTag);
	activeScene->AddGameObject(std::move(tile));
}

void LevelState::ParseEnemies(std::stringstream& levelInfoStream)
{
	std::string enemies{};
	std::getline(levelInfoStream, enemies, ':');
	enemies.erase(0, 1);

	std::stringstream enemiesStream{ enemies };

	std::string enemy{};
	while (std::getline(enemiesStream, enemy))
	{
		std::string zenChan{ "ZenChan" };
		if (enemy.find(zenChan) != std::string::npos)
		{
			std::string element{};
			enemy.erase(0, zenChan.size());
			std::stringstream enemyStream{ enemy };
			glm::vec2 pos{};

			enemyStream >> pos.x >> pos.y;


			spawners::SpawnZenChan(pos,
				m_pPlayerOne ? m_pPlayerOne->GetComponent<PlayerComponent>() : nullptr,
				m_pPlayerTwo ? m_pPlayerTwo->GetComponent<PlayerComponent>() : nullptr);
			++m_AmountOfEnnemiesLeft;
		}

		std::string maita{ "Maita" };
		if (enemy.find(maita) != std::string::npos)
		{
			std::string element{};
			enemy.erase(0, maita.size());
			std::stringstream enemyStream{ enemy };
			glm::vec2 pos{};

			enemyStream >> pos.x >> pos.y;

			spawners::SpawnMaita(pos,
				m_pPlayerOne ? m_pPlayerOne->GetComponent<PlayerComponent>() : nullptr,
				m_pPlayerTwo ? m_pPlayerTwo->GetComponent<PlayerComponent>() : nullptr);
			++m_AmountOfEnnemiesLeft;
		}
	}
}