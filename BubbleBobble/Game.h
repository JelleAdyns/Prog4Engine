#ifndef GAME_H
#define GAME_H

#include <Singleton.h>
#include <AudioService.h>
#include <vector>
#include <algorithm>
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
		std::for_each(m_SceneStack.crbegin(), m_SceneStack.crend(),
			[](const std::unique_ptr<SceneState>& scene)
			{
				scene->OnExit();
			}
		);
		m_SceneStack.clear();
		
		m_SceneStack.push_back(std::make_unique<SceneType>());
		
		m_SceneStack.back()->OnEnter();
	}

	template <typename SceneType>
		requires std::derived_from<SceneType, SceneState>
	void PushScene()
	{
		m_SceneStack.back()->OnSuspend();

		m_SceneStack.push_back(std::make_unique<SceneType>());
		
		m_SceneStack.back()->OnEnter();
	}

	void PopScene()
	{
		m_SceneStack.back()->OnExit();

		m_SceneStack.pop_back();

		m_SceneStack.back()->OnResume();
	}

	void SetGameMode(Game::GameMode newGameMode) { m_CurrentGameMode = newGameMode; }
	GameMode GetCurrentGameMode() {return m_CurrentGameMode;}
private:

	std::unique_ptr<SceneState> m_CurrScene{ nullptr };
	std::unique_ptr<PauseScreenState> m_PauseScreen{ nullptr };
	GameMode m_CurrentGameMode{ GameMode::SinglePlayer };
	std::vector<std::unique_ptr<SceneState>> m_SceneStack;
	friend class dae::Singleton<Game>;
	Game() = default;
};

#endif // !GAME_H