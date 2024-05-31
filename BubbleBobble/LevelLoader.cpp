#include "LevelLoader.h"
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
#include "Components.h"
#include "CollisionTags.h"

namespace levelLoader
{

	void ParseLevelInfo(int levelNumber, std::ifstream& file, std::stringstream& levelInfoStream);
	void ParseStage(int levelNumber, std::stringstream& levelInfoStream);
	void ParseEnemies(std::stringstream& levelInfoStream);



	void LoadLevel(const std::string& filename, int levelNumber)
	{
		std::string path{ dae::ResourceManager::GetInstance().GetDataPath() + filename };

		if (std::ifstream inputFile{ path, std::ios::in }; inputFile.is_open())
		{
			std::string level{};
			
			std::stringstream infoStream{};
			ParseLevelInfo(levelNumber, inputFile, infoStream);

			ParseStage(levelNumber, infoStream);

			ParseEnemies(infoStream);

		}
		else { std::cout << "Couldn't open file: " << filename << '\n'; }
	}




	void ParseLevelInfo(int levelNumber, std::ifstream& file, std::stringstream& levelInfoStream)
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

	void ParseStage(int levelNumber, std::stringstream& levelInfoStream)
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

	void ParseEnemies(std::stringstream& levelInfoStream)
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

				//spawners::SpawnZenChan(pos);
			}

			std::string maita{ "Maita" };
			if (enemy.find(maita) != std::string::npos)
			{
				std::string element{};
				enemy.erase(0, maita.size());
				std::stringstream enemyStream{ enemy };
				glm::vec2 pos{};

				enemyStream >> pos.x >> pos.y;

				//spawners::SpawnMaita(pos);
			}
		}
	}

}

