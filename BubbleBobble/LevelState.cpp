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
#include <Minigin.h>
#include <filesystem>
#include "CollisionTags.h"
#include <KeyState.h>
#include <AudioLocator.h>
#include "Game.h"
#include "HitState.h"
#include "Spawners.h"
#include "ActivateButtonCommand.h"
#include "Achievements.h"

const std::string LevelState::m_SceneName{ "Level" };

void LevelState::OnEnter()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName + std::to_string(m_LevelNumber));
	CreateSkipButton(scene);
	
	UploadScene(scene);

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.AddSound("Sounds/MainTheme.mp3", static_cast<dae::SoundID>(Game::SoundEvent::MainTheme));
	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::MainTheme), 80, true);
}

void LevelState::OnExit()
{
	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.StopAllSounds();
}

void LevelState::OnSuspend()
{
}

void LevelState::OnResume()
{
}

void LevelState::AdvanceLevel()
{
	if(m_LevelNumber == m_MaxLevel)
	{
		Game::GetInstance().SetScene(Game::CurrScene::TitleScreen);
	}
	else
	{
		++m_LevelNumber;
		//m_pPlayerOne->RemoveComponent<MovementComponent>();
		auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName + std::to_string(m_LevelNumber));
		CreateSkipButton(scene);
		UploadScene(scene);
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
	inputMan.RemoveAllCommands();

	std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(pButtonHandler);
	inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_F1, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::Start, dae::KeyState::UpThisFrame, 0);


	scene.AddGameObject(std::move(pButtonHandler));
	scene.AddGameObject(std::move(pButton));
}

void LevelState::MakePlayer(const std::unique_ptr<dae::GameObject>& pPlayer, PlayerComponent::PlayerType playerType, ScoreUIComponent* scoreDisplay, LivesUIComponent* livesDisplay)
{
	
	pPlayer->AddRenderComponent();
	pPlayer->AddPhysicsComponent();
	uint8_t playerIndex{};
	switch (playerType)
	{
	case PlayerComponent::PlayerType::Green:
		playerIndex = 0;
		pPlayer->AddComponent<SpriteComponent>("Textures/BubStates.png", 4, 8, 0.1f, true, false);
		break;
	case PlayerComponent::PlayerType::Blue:
		playerIndex = 1;
		pPlayer->AddComponent<SpriteComponent>("Textures/BobStates.png", 4, 8, 0.1f, true, false);
		pPlayer->GetComponent<SpriteComponent>()->LookLeft(true);
		break;
	}
	pPlayer->AddComponent<MovementComponent>(-160.f, 60.f, playerIndex);
	pPlayer->AddComponent<InventoryComponent>(scoreDisplay);
	dae::PhysicsComponent::SetGravity(300);
	pPlayer->AddComponent<PlayerComponent>(playerType, livesDisplay);
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

void LevelState::CreateScoreDisplay(dae::Scene& scene, bool playerOne)
{
	//Score Display
	float distanceBetweenDisplays{ dae::Minigin::GetWindowSize().x / 4.f };
	auto pScoreDisplayText = std::make_unique<dae::GameObject>(playerOne ? distanceBetweenDisplays : distanceBetweenDisplays * 3, 4.f);
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
	pScoreDisplayText->AddComponent<dae::TextComponent>(displayText, "Fonts/Pixel_NES.otf", 8, playerOne ? glm::u8vec4{ 116, 251, 77, 255 } : glm::u8vec4{ 77, 166, 248 ,255 });


	ScoreUIComponent* pScoreUIComp = pScoreDisplay->GetComponent<ScoreUIComponent>();
	playerOne ? m_pPlayerOne.pScoreUIComp = pScoreUIComp : m_pPlayerTwo.pScoreUIComp = pScoreUIComp;

	scene.AddGameObject(std::move(pScoreDisplayText));
	scene.AddGameObject(std::move(pScoreDisplay));
}

void LevelState::CreateLivesDisplay()
{
}

void LevelState::UploadScene(dae::Scene& scene)
{
	switch (Game::GetInstance().GetCurrentGameMode())
	{
	case Game::GameMode::SinglePlayer:
	{
		if (m_pPlayerOne.pScoreUIComp) m_pPlayerOne.score = m_pPlayerOne.pScoreUIComp->GetScore();

		CreateScoreDisplay(scene, true);
		CreateScoreDisplay(scene, false);

		//Lives Display
		auto pLivesUI = std::make_unique<dae::GameObject>();
		pLivesUI->AddComponent<LivesUIComponent>();
		auto pLivesUIComponent = pLivesUI->GetComponent<LivesUIComponent>();

		//Player
		auto pPlayer = std::make_unique<dae::GameObject>(24.f, dae::Minigin::GetWindowSize().y - 24.f);
		MakePlayer(pPlayer, PlayerComponent::PlayerType::Green, m_pPlayerOne.pScoreUIComp, pLivesUIComponent);

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
		if (m_pPlayerOne.pScoreUIComp) m_pPlayerOne.score = m_pPlayerOne.pScoreUIComp->GetScore();
		if (m_pPlayerTwo.pScoreUIComp) m_pPlayerTwo.score = m_pPlayerTwo.pScoreUIComp->GetScore();

		CreateScoreDisplay(scene, true);
		CreateScoreDisplay(scene, false);

		//Lives Display
		auto pLivesUIOne = std::make_unique<dae::GameObject>();
		pLivesUIOne->AddComponent<LivesUIComponent>();
		auto pLivesUIOneComponent = pLivesUIOne->GetComponent<LivesUIComponent>();

		//Player
		auto pPlayerOne = std::make_unique<dae::GameObject>(24.f, dae::Minigin::GetWindowSize().y - 24.f);
		MakePlayer(pPlayerOne, PlayerComponent::PlayerType::Green, m_pPlayerOne.pScoreUIComp, pLivesUIOneComponent);

	
		//Lives Display
		auto pLivesUITwo = std::make_unique<dae::GameObject>();
		pLivesUITwo->AddComponent<LivesUIComponent>();
		auto pLivesUITwoComponent = pLivesUIOne->GetComponent<LivesUIComponent>();

		//Player
		auto pPlayerTwo = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x - 32.f, dae::Minigin::GetWindowSize().y - 24.f);
		MakePlayer(pPlayerTwo, PlayerComponent::PlayerType::Blue, m_pPlayerTwo.pScoreUIComp, pLivesUITwoComponent);

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
		break;
	}
	

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