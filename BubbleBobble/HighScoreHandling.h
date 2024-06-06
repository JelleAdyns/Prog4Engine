#ifndef HIGHSCOREHANDLING_H
#define HIGHSCOREHANDLING_H

#include <string>
#include <vector>

namespace highScoreHandling
{
	struct PlayerScore
	{
		std::string name;
		int score;
	};

	const std::string filename{ "HighScores.txt" };

	void LoadHighScores(std::vector<PlayerScore>& highscores);
	void WriteHighScores(const std::string& initials, int score);
}

#endif // !HIGHSCOREHANDLING_H
