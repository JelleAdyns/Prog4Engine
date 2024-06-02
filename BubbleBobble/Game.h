#ifndef GAME_H
#define GAME_H

#include <Singleton.h>
#include <AudioService.h>
#include "TitleScreenState.h"
namespace dae
{
	class Scene;
}

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
		Level,
		PauseScreen,
		HighScore,
		DeathScene,
		WelcomeScreen
	};

	// I've got this idea of making sure the underlying type is SoundID from Wout Firlefyn
	enum class SoundEvent : dae::SoundID
	{
		MainTheme, 
		TitleScreen,
		Select,
		Shoot,
		EnemyDeath,
		PickUp,
		Jump,
		JumpBubble
	};


	void StartGame();
	void SetScene(Game::CurrScene newScene);
private:
	void LoadTitleScreen() const;
	void LoadMainMenu() const;
	void LoadLevelOne() const;
	void LoadLevelTwo() const;
	void LoadLevelThree() const;

	void MakePlayer(dae::Scene& scene) const;

	CurrScene m_ActiveScene{ CurrScene::TitleScreen };
	std::unique_ptr<SceneState> m_CurrScene{ nullptr };

	friend class dae::Singleton<Game>;
	Game() = default;
};

#endif // !GAME_H