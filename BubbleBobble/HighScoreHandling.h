#ifndef HIGHSCOREHANDLING_H
#define HIGHSCOREHANDLING_H

#include <string>
#include <vector>
#include <Scene.h>

namespace highScoreHandling
{
	struct PlayerScore
	{
		std::string name;
		int score;
	};

	const std::string fileName{ "HighScores.txt" };
	const unsigned int fontsize{ 10 };
	const std::string fontName{ "Fonts/Pixel_NES.otf" };

	void LoadHighScores(std::vector<PlayerScore>& highscores);
	void WriteHighScores(const std::string& initials, int score);
	bool NameIsInList(const std::string& name);
	PlayerScore GetFirstScore();
	void RemoveHighScore(const std::string& name);
	void CreateScoreLineObjects(dae::Scene& scene, const glm::vec2& pos, const PlayerScore& playerScore, int rank);
}

#endif // !HIGHSCOREHANDLING_H
