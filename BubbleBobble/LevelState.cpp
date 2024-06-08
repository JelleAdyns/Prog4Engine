#include "LevelState.h"
#include "Components.h"
#include "NextLevelCommand.h"
#include <SceneManager.h>
#include <ResourceManager.h>
#include <InputCommandBinder.h>
#include <Scene.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <GameObject.h>
#include <filesystem>
#include "CollisionTags.h"
#include <KeyState.h>
#include <AudioLocator.h>
#include <Renderer.h>
#include "Game.h"
#include "HitState.h"
#include "Spawners.h"
#include "ActivateButtonCommand.h"
#include "Achievements.h"
#include "LoadSceneCommands.h"

const std::string LevelState::m_SceneName{ "Level" };
LevelState::PlayerInfo LevelState::m_pPlayerOne{ .textColor{ 116, 251, 77, 255 }, .spawnPos{24.f, dae::Minigin::GetWindowSize().y - 24.f}  };
LevelState::PlayerInfo LevelState::m_pPlayerTwo{ .textColor{  77, 166, 248 ,255 }, .spawnPos{dae::Minigin::GetWindowSize().x - 40.f, dae::Minigin::GetWindowSize().y - 24.f} };

void LevelState::OnEnter()
{

	m_pPlayerOne.pPlayerObject = nullptr;
	m_pPlayerOne.pLivesUIComp = nullptr;
	m_pPlayerOne.pScoreUIComp = nullptr;
	m_pPlayerOne.score = 0;
	m_pPlayerOne.health = PlayerInfo::startHealth;

	m_pPlayerTwo.pPlayerObject = nullptr;
	m_pPlayerTwo.pLivesUIComp = nullptr;
	m_pPlayerTwo.pScoreUIComp = nullptr;
	m_pPlayerTwo.score = 0;
	m_pPlayerTwo.health = PlayerInfo::startHealth;

	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName + std::to_string(m_LevelNumber));
	CreateSkipButton(scene);
	CreatePauseButton(scene);
	
	UploadScene(scene);

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.AddSound("Sounds/MainTheme.mp3", static_cast<dae::SoundID>(Game::SoundEvent::MainTheme));
	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::MainTheme), 80, true);

	dae::Renderer::GetInstance().StartFadeIn(0.5f);
}

void LevelState::OnExit()
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.StopAllSounds();

	UpdatePlayerInfo();

}

void LevelState::OnSuspend()
{
	dae::SceneManager::GetInstance().SuspendActiveScene();
	dae::InputCommandBinder::GetInstance().DeactivateAllCommands();
}

void LevelState::OnResume()
{
	dae::SceneManager::GetInstance().ResumeSuspendedScene();
	dae::Scene* activeScene = dae::SceneManager::GetInstance().GetActiveScene();

	dae::InputCommandBinder::GetInstance().ActivateAllCommands();
	CreatePauseButton(*activeScene);
}

void LevelState::AdvanceLevel()
{
	UpdatePlayerInfo();

	if(m_LevelNumber == m_MaxLevel)
	{
		Game::GetInstance().SetScene(Game::CurrScene::Results);
	}
	else
	{
		auto& inputMan = dae::InputCommandBinder::GetInstance();
		inputMan.RemoveAllCommands();

		++m_LevelNumber;

		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName + std::to_string(m_LevelNumber));
		CreateSkipButton(scene);
		CreatePauseButton(scene);
		UploadScene(scene);

		dae::Renderer::GetInstance().StartFadeIn(0.5f);
	}	
}

void LevelState::CreateSkipButton(dae::Scene& scene)
{
	auto pButtonHandler = std::make_unique<dae::GameObject>();
	pButtonHandler->AddComponent<ButtonHandlerComponent>();
	auto pHandlerComp = pButtonHandler->GetComponent<ButtonHandlerComponent>();

	std::unique_ptr<dae::Command> pNextLevelCommand = std::make_unique<NextLevelCommand>(this);
	auto pButton = std::make_unique<dae::GameObject>();
	pButton->AddComponent<ButtonComponent>(pNextLevelCommand);
	auto pButtonComp = pButton->GetComponent<ButtonComponent>();

	pHandlerComp->AddButton(pButtonComp);

	auto& inputMan = dae::InputCommandBinder::GetInstance();

	std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(pButtonHandler);
	inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_F1, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::RightThumb, dae::KeyState::UpThisFrame, 0);


	scene.AddGameObject(std::move(pButtonHandler));
	scene.AddGameObject(std::move(pButton));
}

void LevelState::CreatePauseButton(dae::Scene& scene)
{
	auto pButtonHandler = std::make_unique<dae::GameObject>();
	pButtonHandler->AddComponent<ButtonHandlerComponent>();
	auto pHandlerComp = pButtonHandler->GetComponent<ButtonHandlerComponent>();

	std::unique_ptr<dae::Command> pNextLevelCommand = std::make_unique<PushSceneCommand>(Game::CurrScene::PauseScreen);
	auto pButton = std::make_unique<dae::GameObject>();
	pButton->AddComponent<ButtonComponent>(pNextLevelCommand);
	auto pButtonComp = pButton->GetComponent<ButtonComponent>();

	pHandlerComp->AddButton(pButtonComp);

	auto& inputMan = dae::InputCommandBinder::GetInstance();

	std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(pButtonHandler);
	inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_ESCAPE, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::Start, dae::KeyState::UpThisFrame, 0);


	scene.AddGameObject(std::move(pButtonHandler));
	scene.AddGameObject(std::move(pButton));
}

void LevelState::MakePlayer(const std::unique_ptr<dae::GameObject>& pPlayer, PlayerComponent::PlayerType playerType, ScoreUIComponent* scoreDisplay, LivesUIComponent* livesDisplay, PlayerCounterComponent* pCounterComponent)
{
	pPlayer->AddRenderComponent();
	pPlayer->AddPhysicsComponent();
	dae::PhysicsComponent::SetGravity(300);

	uint8_t playerIndex{};
	int startHealth{};
	switch (playerType)
	{
	case PlayerComponent::PlayerType::Green:
		playerIndex = 0;
		startHealth = m_pPlayerOne.health;
		pPlayer->AddComponent<SpriteComponent>("Textures/BubStates.png", 4, 8, 0.1f, true, false);
		break;
	case PlayerComponent::PlayerType::Blue:
		playerIndex = 1;
		startHealth = m_pPlayerTwo.health;
		pPlayer->AddComponent<SpriteComponent>("Textures/BobStates.png", 4, 8, 0.1f, true, false);
		pPlayer->GetComponent<SpriteComponent>()->LookLeft(true);
		break;
	}
	
	pPlayer->AddComponent<MovementComponent>(-160.f, 60.f, playerIndex);
	pPlayer->AddComponent<InventoryComponent>(scoreDisplay);

	pPlayer->AddComponent<PlayerComponent>(playerType, startHealth, livesDisplay, scoreDisplay, pCounterComponent);
	PlayerComponent* playerComp = pPlayer->GetComponent<PlayerComponent>();
	SpriteComponent* spriteComp = pPlayer->GetComponent<SpriteComponent>();
	spriteComp->AddObserver(playerComp);
	spriteComp->SetHeightMarkers(0, HitState::GetHitSpriteOffset());


	const auto& destRctSize = spriteComp->GetDestRectSize();

	pPlayer->AddComponent<dae::CollisionComponent>(
		glm::vec2{ m_GeneralCollisionOffset ,m_GeneralCollisionOffset },
		glm::vec2{ destRctSize.x - m_GeneralCollisionOffset * 2 ,destRctSize.y - m_GeneralCollisionOffset * 2 },
		collisionTags::playerTag);

	pPlayer->AddComponent<WallCheckingComponent>(glm::vec2{ 0,destRctSize.y / 4 }, glm::vec2{ destRctSize.x,destRctSize.y / 2 });
	pPlayer->AddComponent<FloorCheckingComponent>(glm::vec2{ destRctSize.x / 4,0 }, glm::vec2{ destRctSize.x / 2,destRctSize.y });

	MovementComponent* movementComp = pPlayer->GetComponent<MovementComponent>();
	if(movementComp->GetPlayerIndex() == 0) m_pPlayerOne.pPlayerObject = pPlayer.get();
	else m_pPlayerTwo.pPlayerObject = pPlayer.get();

	
}

void LevelState::CreateScoreDisplay(dae::Scene& scene, HighScoreUIComponent* pHighScoreComp, bool playerOne)
{
	//Score Display
	float distanceBetweenDisplays{ dae::Minigin::GetWindowSize().x / 6.f };
	auto pScoreDisplayText = std::make_unique<dae::GameObject>(playerOne ? distanceBetweenDisplays : distanceBetweenDisplays * 5, 4.f);
	pScoreDisplayText->AddRenderComponent(true);

	auto pScoreDisplay = std::make_unique<dae::GameObject>(0, 8);
	pScoreDisplay->AddRenderComponent(true);
	pScoreDisplay->AddComponent<dae::TextComponent>("", "Fonts/Pixel_NES.otf", 8);

	pScoreDisplay->SetParent(pScoreDisplayText, false);

	std::string displayText{"1UP"};
	if (!playerOne)
	{
		switch (Game::GetInstance().GetCurrentGameMode())
		{
		case Game::GameMode::SinglePlayer:
			displayText = "Insert Coin";
			pScoreDisplay->AddComponent<ScoreUIComponent>(m_pPlayerTwo.score, &Achievements::GetInstance());
			break;
		case Game::GameMode::MultiPlayer:
			displayText = "2UP";
			pScoreDisplay->AddComponent<ScoreUIComponent>(m_pPlayerTwo.score, &Achievements::GetInstance());
			break;
		case Game::GameMode::Versus:
			displayText = "Versus";
			pScoreDisplay->AddComponent<ScoreUIComponent>(0, &Achievements::GetInstance());
			break;
		}

	}
	else pScoreDisplay->AddComponent<ScoreUIComponent>(m_pPlayerOne.score, &Achievements::GetInstance());
	pScoreDisplayText->AddComponent<dae::TextComponent>(displayText, "Fonts/Pixel_NES.otf", 8, playerOne ? m_pPlayerOne.textColor : m_pPlayerTwo.textColor);


	ScoreUIComponent* pScoreUIComp = pScoreDisplay->GetComponent<ScoreUIComponent>();
	playerOne ? m_pPlayerOne.pScoreUIComp = pScoreUIComp : m_pPlayerTwo.pScoreUIComp = pScoreUIComp;
	pScoreUIComp->AddObserver(pHighScoreComp);

	scene.AddGameObject(std::move(pScoreDisplayText));
	scene.AddGameObject(std::move(pScoreDisplay));
}

void LevelState::UploadScene(dae::Scene& scene)
{
	// HighScore
	auto pHighScoreDisplayText = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x /2.f, 4.f);
	pHighScoreDisplayText->AddRenderComponent(true);
	pHighScoreDisplayText->AddComponent<dae::TextComponent>("HighScore", "Fonts/Pixel_NES.otf", 8, glm::u8vec4{255,0,0,255});

	auto pHighScoreDisplay = std::make_unique<dae::GameObject>(0, 8);
	pHighScoreDisplay->AddRenderComponent(true);
	pHighScoreDisplay->AddComponent<dae::TextComponent>("", "Fonts/Pixel_NES.otf", 8);
	pHighScoreDisplay->AddComponent<HighScoreUIComponent>();
	pHighScoreDisplay->SetParent(pHighScoreDisplayText, false);

	HighScoreUIComponent* pHighScoreComp = pHighScoreDisplay->GetComponent<HighScoreUIComponent>();

	// PlayerCounter
	std::unique_ptr<dae::Command> pNextCmd = std::make_unique<LoadSceneCommand>(Game::CurrScene::DeathScreen);
	auto pCounter = std::make_unique<dae::GameObject>();
	pCounter->AddComponent<PlayerCounterComponent>(std::move(pNextCmd));
	PlayerCounterComponent* pCounterComp = pCounter->GetComponent<PlayerCounterComponent>();


	switch (Game::GetInstance().GetCurrentGameMode())
	{
	case Game::GameMode::SinglePlayer:
	{

		CreateScoreDisplay(scene, pHighScoreComp, true);
		CreateScoreDisplay(scene, nullptr, false);

		//Lives Display
		auto pLivesUI = std::make_unique<dae::GameObject>();
		pLivesUI->AddComponent<LivesUIComponent>();
		auto pLivesUIComponent = pLivesUI->GetComponent<LivesUIComponent>();
		m_pPlayerOne.pLivesUIComp = pLivesUIComponent;

		//Player
		auto pPlayer = std::make_unique<dae::GameObject>(m_pPlayerOne.spawnPos);
		MakePlayer(pPlayer, PlayerComponent::PlayerType::Green, m_pPlayerOne.pScoreUIComp, m_pPlayerOne.pLivesUIComp, pCounterComp);

		//Level
		LoadLevel("Levels.txt");

		//Lives
		for (int i = 0; i < pPlayer->GetComponent<PlayerComponent>()->GetNrOfLives(); i++)
		{
			auto pLife = std::make_unique<dae::GameObject>(8.f * i, dae::Minigin::GetWindowSize().y - 8.f);
			pLife->AddRenderComponent();
			pLife->AddComponent<SpriteComponent>("Textures/Lives.png", 2, 1, 0.1f, false, false);
			pLivesUIComponent->AddLifeObjct(pLife.get());
			scene.AddGameObject(std::move(pLife));
		}
		
		scene.AddGameObject(std::move(pLivesUI));
		scene.AddGameObject(std::move(pPlayer));
	}
		break;
	case Game::GameMode::MultiPlayer:
	{


		CreateScoreDisplay(scene, pHighScoreComp, true);
		CreateScoreDisplay(scene, pHighScoreComp, false);

		//Lives Display
		auto pLivesUIOne = std::make_unique<dae::GameObject>();
		pLivesUIOne->AddComponent<LivesUIComponent>();
		auto pLivesUIOneComponent = pLivesUIOne->GetComponent<LivesUIComponent>();
		m_pPlayerOne.pLivesUIComp = pLivesUIOneComponent;

		//Player
		auto pPlayerOne = std::make_unique<dae::GameObject>(m_pPlayerOne.spawnPos);
		MakePlayer(pPlayerOne, PlayerComponent::PlayerType::Green, m_pPlayerOne.pScoreUIComp, m_pPlayerOne.pLivesUIComp, pCounterComp);

	
		//Lives Display
		auto pLivesUITwo = std::make_unique<dae::GameObject>();
		pLivesUITwo->AddComponent<LivesUIComponent>();
		auto pLivesUITwoComponent = pLivesUITwo->GetComponent<LivesUIComponent>();
		m_pPlayerTwo.pLivesUIComp = pLivesUITwoComponent;

		//Player
		auto pPlayerTwo = std::make_unique<dae::GameObject>(m_pPlayerTwo.spawnPos);
		MakePlayer(pPlayerTwo, PlayerComponent::PlayerType::Blue, m_pPlayerTwo.pScoreUIComp, m_pPlayerTwo.pLivesUIComp, pCounterComp);

		//Level
		LoadLevel("Levels.txt");
		
		//Lives
		for (int i = 0; i < pPlayerOne->GetComponent<PlayerComponent>()->GetNrOfLives(); i++)
		{
			auto pLife = std::make_unique<dae::GameObject>(8.f * i, dae::Minigin::GetWindowSize().y - 8.f);
			pLife->AddRenderComponent();
			pLife->AddComponent<SpriteComponent>("Textures/Lives.png", 2, 1, 0.1f, false, false);
			pLivesUIOneComponent->AddLifeObjct(pLife.get());
			scene.AddGameObject(std::move(pLife));
		}
		for (int i = 0; i < pPlayerTwo->GetComponent<PlayerComponent>()->GetNrOfLives(); i++)
		{
			auto pLife = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x - 8.f * (i+1), dae::Minigin::GetWindowSize().y - 8.f);
			pLife->AddRenderComponent();
			pLife->AddComponent<SpriteComponent>("Textures/Lives.png", 2, 1, 0.1f, false, false);
			pLife->GetComponent<SpriteComponent>()->SetCol(1);
			pLivesUITwoComponent->AddLifeObjct(pLife.get());
			scene.AddGameObject(std::move(pLife));
		}
		
		scene.AddGameObject(std::move(pLivesUIOne));
		scene.AddGameObject(std::move(pLivesUITwo));

		scene.AddGameObject(std::move(pPlayerOne));
		scene.AddGameObject(std::move(pPlayerTwo));
	}
		break;
	case Game::GameMode::Versus:
	{

		CreateScoreDisplay(scene, pHighScoreComp, true);
		CreateScoreDisplay(scene, nullptr, false);

		//Lives Display
		auto pLivesUI = std::make_unique<dae::GameObject>();
		pLivesUI->AddComponent<LivesUIComponent>();
		auto pLivesUIComponent = pLivesUI->GetComponent<LivesUIComponent>();
		m_pPlayerOne.pLivesUIComp = pLivesUIComponent;

		//Player
		auto pPlayer = std::make_unique<dae::GameObject>(24.f, dae::Minigin::GetWindowSize().y - 24.f);
		MakePlayer(pPlayer, PlayerComponent::PlayerType::Green, m_pPlayerOne.pScoreUIComp, m_pPlayerOne.pLivesUIComp, pCounterComp);

		//Player
		auto pMaitaPlayer = std::make_unique<dae::GameObject>(m_pPlayerTwo.spawnPos);
		CreatePlayableMaita(pMaitaPlayer);

		//Level
		LoadLevel("Levels.txt");

		//Lives
		for (int i = 0; i < pPlayer->GetComponent<PlayerComponent>()->GetNrOfLives(); i++)
		{
			auto pLife = std::make_unique<dae::GameObject>(8.f * i, dae::Minigin::GetWindowSize().y - 8.f);
			pLife->AddRenderComponent();
			pLife->AddComponent<SpriteComponent>("Textures/Lives.png", 2, 1, 0.1f, false, false);
			pLivesUIComponent->AddLifeObjct(pLife.get());
			scene.AddGameObject(std::move(pLife));
		}

		scene.AddGameObject(std::move(pLivesUI));
		scene.AddGameObject(std::move(pPlayer));
		scene.AddGameObject(std::move(pMaitaPlayer));
	}
		break;
	}
	
	scene.AddGameObject(std::move(pHighScoreDisplayText));
	scene.AddGameObject(std::move(pHighScoreDisplay));
	scene.AddGameObject(std::move(pCounter));
}

void LevelState::LoadLevel(const std::string& filename)
{
	std::string path{ dae::ResourceManager::GetInstance().GetDataPath() + filename };

	if (std::ifstream inputFile{ path, std::ios::in }; inputFile.is_open())
	{
		std::string level{};

		std::stringstream infoStream{};
		ParseLevelInfo(m_LevelNumber, inputFile, infoStream);

		ParseStage(m_LevelNumber, infoStream);

		ParseEnemies(infoStream);

	}
	else { std::cout << "Couldn't open file: " << filename << '\n'; }
}

void LevelState::ParseLevelInfo(int levelNumber, std::ifstream& file, std::stringstream& levelInfoStream)
{

	std::string levelInfo{};

	while (std::getline(file, levelInfo, '-'))
	{
		if (levelInfo.find("Level " + std::to_string(levelNumber)) != std::string::npos)
		{
			levelInfo.erase(0, levelInfo.find("01"));
			levelInfoStream << levelInfo;
			break;
		}
	}


}

void LevelState::ParseStage(int levelNumber, std::stringstream& levelInfoStream)
{
	std::string level{};

	std::getline(levelInfoStream, level, ':');

	std::stringstream levelStream{ level };
	std::string line{};

	dae::Scene* activeScene = dae::SceneManager::GetInstance().GetNextScene();

	int row{};
	while (std::getline(levelStream, line))
	{
		if (line.size() >= 2) line.erase(0, 2);
		std::stringstream sStream{ line };

		int col{};

		std::for_each(line.cbegin(), line.cend(), [&](char character)
			{
				auto spawnTile = [col, row, levelNumber, &activeScene](const std::string& collisionTag)
					{
						auto pTile = std::make_unique<dae::GameObject>();
						pTile->AddRenderComponent();
						pTile->AddComponent<SpriteComponent>("Textures/Tiles.png", 3, 1, 0.1f, false);

						auto destRect = pTile->GetComponent<SpriteComponent>()->GetDestRectSize();
						pTile->SetLocalPos(static_cast<float>(destRect.x) * col, static_cast<float>(destRect.x) * row);

						if (collisionTag.size() > 0) pTile->AddComponent<dae::CollisionComponent>(glm::vec2{ }, glm::vec2{ destRect.x,destRect.y }, collisionTag);

						pTile->GetComponent<SpriteComponent>()->SetCol(levelNumber - 1);
						activeScene->AddGameObject(std::move(pTile));
					};

				switch (character)
				{
				case 'p':
					spawnTile(collisionTags::platformTag);
					break;
				case 'w':
					spawnTile(collisionTags::wallTag);
					break;
				case 'f':
					spawnTile("");
					break;
				}
				++col;
			});

		++row;
	}

	float extraHeightToBeSafe{ 100.f };
	float wallThickness{ 16.f };

	auto tile = std::make_unique<dae::GameObject>(0.f, -extraHeightToBeSafe);
	tile->AddComponent<dae::CollisionComponent>(glm::vec2{ }, glm::vec2{ wallThickness,extraHeightToBeSafe + wallThickness }, collisionTags::wallTag);
	activeScene->AddGameObject(std::move(tile));

	tile = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x - wallThickness, -extraHeightToBeSafe);
	tile->AddComponent<dae::CollisionComponent>(glm::vec2{ }, glm::vec2{ wallThickness, extraHeightToBeSafe + wallThickness }, collisionTags::wallTag);
	activeScene->AddGameObject(std::move(tile));
}

void LevelState::ParseEnemies(std::stringstream& levelInfoStream)
{
	std::unique_ptr<dae::Command> pNextCmd = std::make_unique<NextLevelCommand>(this);
	auto pCounter = std::make_unique<dae::GameObject>();
	pCounter->AddComponent<EnemyCounterComponent>(std::move(pNextCmd));
	EnemyCounterComponent* pCounterComp = pCounter->GetComponent<EnemyCounterComponent>();

	std::string enemy{};
	std::regex reg{ "^(\\D+): (\\d+), (\\d+)$" };
	while (std::getline(levelInfoStream, enemy))
	{
		std::smatch matches{};
		std::string zenChan{ "ZenChan" };
		std::string maita{ "Maita" };
		if (std::regex_search(enemy.cbegin(), enemy.cend(), matches, reg))
		{
			glm::vec2 pos{};
			pos.x = std::stof(matches.str(2));
			pos.y = std::stof(matches.str(3));
			if (matches.str(1) == zenChan)
			{
				spawners::SpawnZenChan(pos,
					m_pPlayerOne.pPlayerObject ? m_pPlayerOne.pPlayerObject->GetComponent<PlayerComponent>() : nullptr,
					m_pPlayerTwo.pPlayerObject ? m_pPlayerTwo.pPlayerObject->GetComponent<PlayerComponent>() : nullptr,
					pCounterComp);
			}
			if (matches.str(1) == maita)
			{
				spawners::SpawnMaita(pos,
					m_pPlayerOne.pPlayerObject ? m_pPlayerOne.pPlayerObject->GetComponent<PlayerComponent>() : nullptr,
					m_pPlayerTwo.pPlayerObject ? m_pPlayerTwo.pPlayerObject->GetComponent<PlayerComponent>() : nullptr,
					pCounterComp);
			}
		}
	}
	
	dae::SceneManager::GetInstance().GetNextScene()->AddGameObject(std::move(pCounter));
}

void LevelState::CreatePlayableMaita(const std::unique_ptr<dae::GameObject>& pPlayer)
{
	pPlayer->AddRenderComponent();
	pPlayer->AddPhysicsComponent();
	pPlayer->AddComponent<MovementComponent>(-160.f, 60.f, static_cast<uint8_t>(1));
	pPlayer->AddComponent<EnemyComponent>(EnemyComponent::EnemyType::Maita, nullptr);
	float maitaOffset = EnemyComponent::GetMaitaOffset();
	pPlayer->AddComponent<SpriteComponent>("Textures/MaitaStates.png", 5, 9, 0.1f);
	SpriteComponent* maitaSpriteComp = pPlayer->GetComponent<SpriteComponent>();

	auto maitaDestRctSize = maitaSpriteComp->GetDestRectSize();
	float collisionOffset{ LevelState::GetCollisionOffset() };

	pPlayer->AddComponent<dae::CollisionComponent>(
		glm::vec2{ maitaOffset + collisionOffset  ,collisionOffset },
		glm::vec2{ maitaDestRctSize.x - collisionOffset * 2 - maitaOffset * 2 ,maitaDestRctSize.y - collisionOffset * 2 },
		collisionTags::enemyTag);

	pPlayer->AddComponent<WallCheckingComponent>(glm::vec2{ maitaOffset ,maitaDestRctSize.y / 4 }, glm::vec2{ maitaDestRctSize.x - maitaOffset * 2,maitaDestRctSize.y / 2 });
	pPlayer->AddComponent<FloorCheckingComponent>(glm::vec2{ (maitaDestRctSize.x - maitaOffset * 2) / 4 + maitaOffset, 0 }, glm::vec2{ (maitaDestRctSize.x - maitaOffset * 2) / 2,maitaDestRctSize.y });
}

void LevelState::UpdatePlayerInfo()
{
	if (m_pPlayerOne.pScoreUIComp) m_pPlayerOne.score = m_pPlayerOne.pScoreUIComp->GetScore();
	if (m_pPlayerOne.pLivesUIComp) m_pPlayerOne.health = m_pPlayerOne.pLivesUIComp->GetRemainingLives();
	if (m_pPlayerOne.health == 0) m_pPlayerOne.health = 1;

	if (m_pPlayerTwo.pScoreUIComp) m_pPlayerTwo.score = m_pPlayerTwo.pScoreUIComp->GetScore();
	if (m_pPlayerTwo.pLivesUIComp) m_pPlayerTwo.health = m_pPlayerTwo.pLivesUIComp->GetRemainingLives();
	if (m_pPlayerTwo.health == 0) m_pPlayerTwo.health = 1;
}
