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

	enum class GameMode
	{
		SinglePlayer,
		MultiPlayer,
		Versus
	};

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
	void SetGameMode(Game::GameMode newGameMode);
	GameMode GetCurrentGameMode() {return m_CurrentGameMode;}
private:

	std::unique_ptr<SceneState> m_CurrScene{ nullptr };
	GameMode m_CurrentGameMode{ GameMode::SinglePlayer };

	friend class dae::Singleton<Game>;
	Game() = default;
};

#endif // !GAME_H