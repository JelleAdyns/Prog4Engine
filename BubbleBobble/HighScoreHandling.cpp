#include "HighScoreHandling.h"
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
		std::string filePath{ dae::ResourceManager::GetInstance().GetDataPath() + filename };
		if (std::ifstream inputtFile{ dae::ResourceManager::GetInstance().GetDataPath() + filename, std::ios::in }; inputtFile.is_open())
		{
			std::string line{};
			std::regex reg{ "^(\\D{3}),(\\d+)$" };
			while (std::getline(inputtFile, line))
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
		assert((initials.size() == 3) && "The initilials string was not 3 characters long while trying to write to highscores.");

		std::vector<PlayerScore> highScores{};
		LoadHighScores(highScores);
		highScores.push_back(PlayerScore{ initials, score });

		std::sort(highScores.begin(), highScores.end(),
			[](const PlayerScore& firstStudent, const PlayerScore& secondStudent)
			{
				return firstStudent.score > secondStudent.score;
			});

		std::string filePath{ dae::ResourceManager::GetInstance().GetDataPath() + filename };
		if (std::ofstream outputFile{ filePath, std::ios::out }; outputFile.is_open())
		{
			std::copy(highScores.cbegin(), highScores.cend(), std::ostream_iterator<PlayerScore>(outputFile, "\n"));
		}
		else throw std::runtime_error("File path (" + filePath + ") was not recognized.");
	}
}
