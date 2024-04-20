#ifndef GAME_H
#define GAME_H

#include <Singleton.h>

class GameObject;

class Game final : public dae::Singleton<Game>
{
public:

	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	Game& operator= (const Game&) = delete;
	Game& operator= (Game&&) noexcept = delete;

	enum class CurrScene
	{
		TitleScreen,
		Menu,
		Level1,
		Level2,
		Level3,
		HighScore
	};

	void StartGame();
	void SetScene(Game::CurrScene newScene);
private:
	void LoadTitleScreen() const;
	void LoadMainMenu() const;
	void LoadLevelOne() const;
	void LoadLevelTwo() const;
	void LoadLevelThree() const;

	CurrScene m_ActiveScene{ CurrScene::TitleScreen };

	friend class dae::Singleton<Game>;
	Game() = default;
};

#endif // !GAME_H