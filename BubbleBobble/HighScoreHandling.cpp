#include "HighScoreHandling.h"
#include "TextComponent.h"
#include <fstream>
#include <regex>
#include <algorithm>
#include <ResourceManager.h>

namespace highScoreHandling
{
	std::ostream& operator<<(std::ostream& out, const PlayerScore& player)
	{
		return out << player.name << ',' << player.score;
	}

	
	void LoadHighScores(std::vector<PlayerScore>& highscores)
	{
		std::string filePath{ dae::ResourceManager::GetInstance().GetDataPath() + fileName };
		if (std::ifstream inputFile{ dae::ResourceManager::GetInstance().GetDataPath() + fileName, std::ios::in }; inputFile.is_open())
		{
			std::string line{};
			std::regex reg{ "^(\\D{3}),(\\d+)$" };
			while (std::getline(inputFile, line))
			{
				std::smatch matches{};
				if (std::regex_search(line.cbegin(), line.cend(), matches, reg))
				{
					highscores.push_back(PlayerScore{ matches.str(1), std::stoi(matches.str(2))});
				}
#ifndef NDEBUG
				else
				{
					std::cout << "Invalid line: " << line << '\n';
				}
#endif // !NDEBUG

			}
		}
		else throw std::runtime_error("File path (" + filePath + ") was not recognized.");
	}

	void WriteHighScores(const std::string& initials, int score)
	{
		assert((initials.size() <= 3) && "The initials string was longer than 3 characters while trying to write to highscores.");

		std::vector<PlayerScore> highScores{};
		LoadHighScores(highScores);
		highScores.push_back(PlayerScore{ initials, score });

		std::sort(highScores.begin(), highScores.end(),
			[](const PlayerScore& firstStudent, const PlayerScore& secondStudent)
			{
				return firstStudent.score > secondStudent.score;
			});

		std::string filePath{ dae::ResourceManager::GetInstance().GetDataPath() + fileName };
		if (std::ofstream outputFile{ filePath, std::ios::out }; outputFile.is_open())
		{
			std::copy(highScores.cbegin(), highScores.cend(), std::ostream_iterator<PlayerScore>(outputFile, "\n"));
		}
		else throw std::runtime_error("File path (" + filePath + ") was not recognized.");
	}

	bool NameIsInList(const std::string& name)
	{
		std::vector<PlayerScore> highScores{};
		LoadHighScores(highScores);

		return std::any_of(highScores.cbegin(), highScores.cend(), [&](const PlayerScore& playerScore) { return name == playerScore.name; });
	}

	PlayerScore GetFirstScore()
	{
		std::string filePath{ dae::ResourceManager::GetInstance().GetDataPath() + fileName };
		if (std::ifstream inputFile{ dae::ResourceManager::GetInstance().GetDataPath() + fileName, std::ios::in }; inputFile.is_open())
		{
			std::string firstLine{};
			std::getline(inputFile, firstLine);
			std::regex reg{ "^(\\D{3}),(\\d+)$" };

			std::smatch matches{};
			if (std::regex_search(firstLine.cbegin(), firstLine.cend(), matches, reg))
			{
				return PlayerScore{ matches.str(1), std::stoi(matches.str(2)) };
			}
		}
		else throw std::runtime_error("File path (" + filePath + ") was not recognized.");
		return PlayerScore{};
	}

	void RemoveHighScore(const std::string& name)
	{
		std::vector<PlayerScore> highScores{};
		LoadHighScores(highScores);

		highScores.erase(
			std::remove_if(highScores.begin(), highScores.end(), [&](const PlayerScore& playerScore) { return name == playerScore.name; }),
			highScores.end());

		std::string filePath{ dae::ResourceManager::GetInstance().GetDataPath() + fileName };
		if (std::ofstream outputFile{ filePath, std::ios::out }; outputFile.is_open())
		{
			std::copy(highScores.cbegin(), highScores.cend(), std::ostream_iterator<PlayerScore>(outputFile, "\n"));
		}
		else throw std::runtime_error("File path (" + filePath + ") was not recognized.");
		
	}
	void CreateScoreLineObjects(dae::Scene& scene, const glm::vec2& pos, const highScoreHandling::PlayerScore& playerScore, int rank)
	{

		std::string rankName{ std::to_string(rank) };
		switch (rank)
		{
		case 1:
			rankName += "ST";
			break;
		case 2:
			rankName += "ND";
			break;
		case 3:
			rankName += "RD";
			break;
		default:
			rankName += "TH";
			break;
		}
		auto pLine = std::make_unique<dae::GameObject>(pos);
		pLine->AddComponent<dae::TextComponent>(rankName, fontName, fontsize);
		pLine->AddRenderComponent(true);

		auto pScore = std::make_unique<dae::GameObject>(40, 0);
		pScore->AddRenderComponent(true);
		pScore->AddComponent<dae::TextComponent>(std::to_string(playerScore.score), fontName, fontsize);
		pScore->SetParent(pLine, false);

		auto pName = std::make_unique<dae::GameObject>(40, 0);
		pName->AddRenderComponent(true);
		pName->AddComponent<dae::TextComponent>(playerScore.name, fontName, fontsize);
		pName->SetParent(pScore, false);

		scene.AddGameObject(std::move(pLine));
		scene.AddGameObject(std::move(pScore));
		scene.AddGameObject(std::move(pName));
	}
}
