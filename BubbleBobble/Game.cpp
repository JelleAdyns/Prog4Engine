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
#include "LevelLoader.h"
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

void Game::StartGame()
{

#ifndef NDEBUG
	dae::AudioLocator::RegisterAudioService(std::make_unique<dae::LogAudio>(std::make_unique<dae::SDLAudio>()));
#else
	dae::AudioLocator::RegisterAudioService(std::make_unique<dae::SDLAudio>());
#endif // !NDEBUG

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.AddSound("Sounds/TitleScreen.wav", static_cast<dae::SoundID>(Game::SoundEvent::TitleScreen));
	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::TitleScreen), 80, false);

	m_CurrScene = std::make_unique<TitleScreenState>();
	m_CurrScene->OnEnter();
}

void Game::SetScene(Game::CurrScene newScene)
{

	switch (newScene)
	{
	case CurrScene::TitleScreen:
		m_CurrScene->OnExit();
		m_CurrScene = std::make_unique<TitleScreenState>();
		m_CurrScene->OnEnter();
		break;
	case CurrScene::Menu:
		m_CurrScene->OnExit();
		m_CurrScene = std::make_unique<MainMenuState>();
		m_CurrScene->OnEnter();
		break;
	case CurrScene::Level:
		m_CurrScene->OnExit();
		m_CurrScene = std::make_unique<LevelState>();
		m_CurrScene->OnEnter();
		break;
	case CurrScene::HighScore:
		break;
	}

}

void Game::SetGameMode(Game::GameMode newGameMode)
{
	m_CurrentGameMode = newGameMode;
}

