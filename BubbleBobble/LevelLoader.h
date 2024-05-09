#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <ResourceManager.h>
#include <Scene.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <GameObject.h>
#include <filesystem>
#include "Components.h"

static void LoadLevel(const std::string& filename, dae::Scene& scene, int levelNumber)
{
	std::string path{ dae::ResourceManager::GetInstance().GetDataPath() + filename };
	//std::ifstream inputFile{ path, std::ios::in };
	if(std::ifstream inputFile{ path, std::ios::in }; inputFile.is_open())
	{
		std::string line{};
		std::string level{};
		std::stringstream infoStream{};

		while (std::getline(inputFile, level, '-'))
		{
			if (level.find("Level " + std::to_string(levelNumber)) != std::string::npos)
			{
				level.erase(0, level.find("01"));
				infoStream << level;
			}
		}

		int row{};
		while (std::getline(infoStream, line))
		{
			if(line.size() >= 2) line.erase(0,2);
			std::stringstream sStream{ line };

			int col{};

			std::for_each(line.cbegin(), line.cend(), [&](char character)
				{
					switch (character)
					{
					case 'p':
					{
						auto tile = std::make_unique<dae::GameObject>(8.f * col, 8.f * row);
						tile->AddRenderComponent();
						tile->AddComponent<dae::CollisionComponent>(glm::vec2{ 0,0 }, glm::vec2{ 8.f,8.f }, dae::CollisionComponent::CollisionType::Platform);
						tile->AddComponent<SpriteComponent>("Tiles.png", 3, 3, 0.1f, false);

						tile->GetComponent<SpriteComponent>()->SetFrame(levelNumber - 1, 0);
						scene.AddGameObject(std::move(tile));
					}
						break;

					case 'w':
					{
						auto tile = std::make_unique<dae::GameObject>(8.f * col, 8.f * row);
						tile->AddRenderComponent();
						tile->AddComponent<dae::CollisionComponent>(glm::vec2{ 0,0 }, glm::vec2{ 8.f,8.f }, dae::CollisionComponent::CollisionType::Wall);
						tile->AddComponent<SpriteComponent>("Tiles.png", 3, 3, 0.1f, false);

						tile->GetComponent<SpriteComponent>()->SetFrame(levelNumber - 1, 0);
						scene.AddGameObject(std::move(tile));
					}
						break;
					}
					++col;
				});

			++row;
		}
	}
	else { std::cout << "Couldn't open file: " << filename << '\n'; }


	/*while (std::getline(inputFile, line))
	{
		if (line == "Level " + std::to_string(levelNumber))
		{
            std::string nextline{};
            std::regex reg{ "^Tile (\\d+),(\\d+) (\\d+)$" };
            while (std::getline(inputFile, line))
            {
                std::smatch matches{};
                if (std::regex_search(line.cbegin(), line.cend(), matches, reg))
                {
                    for (int i = 0; i < std::stoi(matches.str(3)) ; i++)
                    {
                        auto tile = std::make_unique<dae::GameObject>(std::stof(matches.str(1)) + (8 * i), std::stof(matches.str(2)));
                        tile->AddRenderComponent();
                        tile->AddComponent<dae::SpriteComponent>("Tiles.png", 3, 3, 0.1f, false);

                        scene.AddGameObject(std::move(tile));
                    }
                }
            }
		}
	}*/
}

#endif // !LEVELLOADER_H