#include "Game.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputCommandBinder.h"
#include "Controller.h"
#include "Scene.h" 
#include "Components.h"
#include "Commands.h"
#include "KeyState.h"
#include "GameObject.h"
#include "Achievements.h"
#include "CollisionTags.h"
#include "MainMenuState.h"
#include "LevelState.h"
#include <Minigin.h>
#include <SceneManager.h>
#include <functional>
#include <AudioLocator.h>
#include <SDLAudio.h>
#include <LogAudio.h>

//#pragma warning(push)
//#pragma warning (disable: 4996)
//#include <steam_api.h>
//#pragma warning(pop)

#include <iostream>
#include "ResultsState.h"

void Game::StartGame()
{

#ifndef NDEBUG
	dae::AudioLocator::RegisterAudioService(std::make_unique<dae::LogAudio>(std::make_unique<dae::SDLAudio>()));
#else
	dae::AudioLocator::RegisterAudioService(std::make_unique<dae::SDLAudio>());
#endif // !NDEBUG


	dae::InputCommandBinder::GetInstance().AddController();

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.AddSound("Sounds/TitleScreen.wav", static_cast<dae::SoundID>(Game::SoundEvent::TitleScreen));
	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::TitleScreen), 80, false);

	m_CurrScene = std::make_unique<TitleScreenState>();
	m_CurrScene->OnEnter();
}

void Game::SetScene(Game::CurrScene newScene)
{
	m_CurrScene->OnExit();
	switch (newScene)
	{
	case CurrScene::TitleScreen:
		m_CurrScene = std::make_unique<TitleScreenState>();
		break;
	case CurrScene::Menu:
		m_CurrScene = std::make_unique<MainMenuState>();
		break;
	case CurrScene::Level:
		m_CurrScene = std::make_unique<LevelState>();
		break;
	case CurrScene::Results:
		m_CurrScene = std::make_unique<ResultsState>();
		break;
	}
	m_CurrScene->OnEnter();
}

void Game::SetGameMode(Game::GameMode newGameMode)
{
	m_CurrentGameMode = newGameMode;
}

