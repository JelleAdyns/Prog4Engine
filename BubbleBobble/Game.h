#ifndef GAME_H
#define GAME_H

#include <Singleton.h>
#include <AudioService.h>
#include <stack>
#include "TitleScreenState.h"
#include "PauseScreenState.h"
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
		JumpBubble,
		Boulder
	};


	void StartGame();


	template <typename SceneType>
		requires std::derived_from<SceneType ,SceneState>
	void SetScene()
	{
		while (!m_SceneStack.empty())
		{
			m_SceneStack.top()->OnExit();
			m_SceneStack.pop();
		}

		m_SceneStack.push(std::make_unique<SceneType>());
		m_SceneStack.top()->OnEnter();
	}

	template <typename SceneType>
		requires std::derived_from<SceneType, SceneState>
	void PushScene()
	{
		m_SceneStack.top()->OnSuspend();
		m_SceneStack.push(std::make_unique<SceneType>());
		m_SceneStack.top()->OnEnter();
	}

	void PopScene()
	{
		m_SceneStack.top()->OnExit();
		m_SceneStack.pop();
		m_SceneStack.top()->OnResume();
	}

	void SetGameMode(Game::GameMode newGameMode) { m_CurrentGameMode = newGameMode; }
	GameMode GetCurrentGameMode() {return m_CurrentGameMode;}
private:

	std::unique_ptr<SceneState> m_CurrScene{ nullptr };
	std::unique_ptr<PauseScreenState> m_PauseScreen{ nullptr };
	GameMode m_CurrentGameMode{ GameMode::SinglePlayer };
	std::stack<std::unique_ptr<SceneState>> m_SceneStack;
	friend class dae::Singleton<Game>;
	Game() = default;
};

#endif // !GAME_H