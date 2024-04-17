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
#include <Minigin.h>
#include <functional>



//#pragma warning(push)
//#pragma warning (disable: 4996)
//#include <steam_api.h>
//#pragma warning(pop)

#include <iostream>

void Game::StartGame() const
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& scene = sceneManager.CreateScene("TitleScreen");

	sceneManager.SetActiveScene("TitleScreen");

	auto titleLogo = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, static_cast<float>(dae::Minigin::GetWindowSize().y) / 2);
	titleLogo->AddRenderComponent(true);
	titleLogo->AddComponent<dae::SpriteComponent>("TitleLogo.png", 6, 6, 0.2f);

	scene.AddGameObject(std::move(titleLogo));


	auto& inputMan = dae::InputCommandBinder::GetInstance();
	std::unique_ptr<dae::Command> loadCommand = std::make_unique<dae::LoadSceneCommand>("LevelOne");
	inputMan.AddKeyCommand(std::move(loadCommand), SDL_SCANCODE_L, dae::KeyState::Pressed);

	loadCommand = std::make_unique<dae::LoadSceneCommand>("LevelTwo");
	inputMan.AddKeyCommand(std::move(loadCommand), SDL_SCANCODE_M, dae::KeyState::Pressed);

	loadCommand = std::make_unique<dae::LoadSceneCommand>("LevelThree");
	inputMan.AddKeyCommand(std::move(loadCommand), SDL_SCANCODE_N, dae::KeyState::Pressed);

	LoadLevelOne();
	LoadLevelTwo();
	LoadLevelThree();
}

void Game::LoadLevelOne() const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	//inputMan.RemoveAllCommands();

	//std::unique_ptr<dae::Command> DestroyCommand = std::make_unique<dae::DestroySceneCommand>("TitleScreen");
	//DestroyCommand->Execute();

	auto& sceneManager = dae::SceneManager::GetInstance();

	auto& scene = sceneManager.CreateScene("LevelOne");

	
	//auto gui = std::make_unique<dae::GameObject>();
	//gui->AddComponent<dae::CacheExperimentComponent>();
	//gui->AddComponent<dae::TrashTheCacheIntComponent>(10, "Exercise 1");
	//scene.Add(std::move(gui));

	//auto ggui = std::make_unique<dae::GameObject>();
	//ggui->AddComponent<dae::CacheExperimentComponent>();
	//ggui->AddComponent<dae::TrashTheCacheGameObjectsComponent>(10, "Exercise 2");
	//scene.Add(std::move(ggui));

	//Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddRenderComponent();
	go->AddComponent<dae::ImageComponent>("background.tga");

	scene.AddGameObject(std::move(go));

	//DAE-Image
	go = std::make_unique<dae::GameObject>(100.f, 40.f);
	go->AddRenderComponent();
	go->AddComponent<dae::ImageComponent>("logo.tga");


	//auto gob = std::make_unique<dae::GameObject>(10.f, 10.f);
	//gob->AddRenderComponent();
	//gob->AddComponent<dae::ImageComponent>("logo.tga");

	//gob->SetParent(go, false);
	//// REMOVING CHILD FROM PARENT
	//gob->SetParent(nullptr, true);

	scene.AddGameObject(std::move(go));
	//scene.Add(std::move(gob));


	////Info
	go = std::make_unique<dae::GameObject>(2.f, 28.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Move Bub with WASD, Press E to die and R and T to pick stuff up.", "Lingua.otf", 8);

	scene.AddGameObject(std::move(go));

	go = std::make_unique<dae::GameObject>(2.f, 35.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Move Bob with the DPAD, Press X to die and A and B to pick stuff up.", "Lingua.otf", 8);

	scene.AddGameObject(std::move(go));

	go = std::make_unique<dae::GameObject>(15.f, 210.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("(Specify frame cap in the initialization of the Minigin)", "Lingua.otf", 8);

	scene.AddGameObject(std::move(go));

	go = std::make_unique<dae::GameObject>(15.f, 200.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Press space to toggle frame cap", "Lingua.otf", 8);

	scene.AddGameObject(std::move(go));


	//Title
	go = std::make_unique<dae::GameObject>(30.f, 15.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", "Lingua.otf", 8);

	scene.AddGameObject(std::move(go));

	//FPSCounter
	auto fpsCounter = std::make_unique<dae::GameObject>();
	fpsCounter->AddRenderComponent();
	fpsCounter->AddComponent<dae::TextComponent>("0 FPS", "Lingua.otf", 8);
	fpsCounter->AddComponent<dae::FPSComponent>();

	scene.AddGameObject(std::move(fpsCounter));

	// Rotating Sprites
	{

		auto lo = std::make_unique<dae::GameObject>(75.f, 75.f);


		auto ro = std::make_unique<dae::GameObject>();
		ro->AddRenderComponent(true);
		ro->AddComponent<dae::ImageComponent>("enemy.png");
		ro->AddComponent<dae::RotatorComponent>(20.f, 3.f);

		ro->SetParent(lo, true);

		auto so = std::make_unique<dae::GameObject>();
		so->AddRenderComponent(true);
		so->AddComponent<dae::ImageComponent>("enemy2.png");
		so->AddComponent<dae::RotatorComponent>(20.f, -5.f);

		so->SetParent(ro, true);

		scene.AddGameObject(std::move(lo));
		scene.AddGameObject(std::move(ro));

		//so->SetParent(nullptr, true);
		auto bo = std::make_unique<dae::GameObject>();
		bo->AddRenderComponent(true);
		bo->AddComponent<dae::ImageComponent>("enemy.png");
		bo->AddComponent<dae::RotatorComponent>(30.f, 8.f);

		bo->SetParent(so, false);
		scene.AddGameObject(std::move(bo));
		scene.AddGameObject(std::move(so));
	}

	//Input Objects
	{
		int speedObject1 = 80;
		int speedObject2 = speedObject1 * 2;

		int starterLives = 3;



		//PLAYER 1
		auto livesDisplay = std::make_unique<dae::GameObject>(2.f, 50.f);
		livesDisplay->AddRenderComponent();
		livesDisplay->AddComponent<dae::LivesUIComponent>();
		livesDisplay->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 8);

		auto scoreDisplay = std::make_unique<dae::GameObject>(0.f, 10.f);
		scoreDisplay->AddRenderComponent();
		scoreDisplay->AddComponent<dae::ScoreUIComponent>(&dae::Achievements::GetInstance());
		scoreDisplay->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 8);

		scoreDisplay->SetParent(livesDisplay, false);

		auto player1 = std::make_unique<dae::GameObject>(38.f, 50.f);
		player1->AddRenderComponent();
		player1->AddComponent<dae::LivesComponent>(starterLives, livesDisplay->GetComponent<dae::LivesUIComponent>());
		player1->AddComponent<dae::PickUpComponent>(dae::PickUpComponent::PickUpType::Melon, scoreDisplay->GetComponent<dae::ScoreUIComponent>());
		player1->AddComponent<dae::SpriteComponent>("BubRunning.png", 4, 4, 0.1f);

		std::unique_ptr<dae::Command> killCommand = std::make_unique<dae::KillCommand>(player1);
		inputMan.AddKeyCommand(std::move(killCommand), SDL_SCANCODE_E, dae::KeyState::UpThisFrame);

		std::unique_ptr<dae::Command> pickUpCommand = std::make_unique<dae::PickUpCommand>(player1);
		inputMan.AddKeyCommand(std::move(pickUpCommand), SDL_SCANCODE_R, dae::KeyState::UpThisFrame);

		std::unique_ptr<dae::Command> pickUpCommand2 = std::make_unique<dae::PickUpCommand>(player1);
		inputMan.AddKeyCommand(std::move(pickUpCommand2), SDL_SCANCODE_T, dae::KeyState::UpThisFrame);


		scene.AddGameObject(std::move(livesDisplay));
		scene.AddGameObject(std::move(scoreDisplay));



		//PLAYER 2
		auto livesDisplay2 = std::make_unique<dae::GameObject>(2.f, 100.f);
		livesDisplay2->AddRenderComponent();
		livesDisplay2->AddComponent<dae::LivesUIComponent>();
		livesDisplay2->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 8);

		auto scoreDisplay2 = std::make_unique<dae::GameObject>(0.f, 10.f);
		scoreDisplay2->AddRenderComponent();
		scoreDisplay2->AddComponent<dae::ScoreUIComponent>(&dae::Achievements::GetInstance());
		scoreDisplay2->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 8);

		scoreDisplay2->SetParent(livesDisplay2, false);

		auto player2 = std::make_unique<dae::GameObject>(38.f, 100.f);
		player2->AddRenderComponent();
		player2->AddComponent<dae::LivesComponent>(starterLives, livesDisplay2->GetComponent<dae::LivesUIComponent>());
		player2->AddComponent<dae::PickUpComponent>(dae::PickUpComponent::PickUpType::Fries, scoreDisplay2->GetComponent<dae::ScoreUIComponent>());
		player2->AddComponent<dae::SpriteComponent>("BobRunning.png", 4, 4, 0.1f);

		inputMan.AddController();


		std::unique_ptr<dae::Command> killCommand2 = std::make_unique<dae::KillCommand>(player2);
		inputMan.AddControllerCommand(std::move(killCommand2), dae::ControllerButton::X, dae::KeyState::UpThisFrame, 0);

		pickUpCommand = std::make_unique<dae::PickUpCommand>(player2);
		inputMan.AddControllerCommand(std::move(pickUpCommand), dae::ControllerButton::A, dae::KeyState::UpThisFrame, 0);

		pickUpCommand2 = std::make_unique<dae::PickUpCommand>(player2);
		inputMan.AddControllerCommand(std::move(pickUpCommand2), dae::ControllerButton::B, dae::KeyState::UpThisFrame, 0);


		scene.AddGameObject(std::move(livesDisplay2));
		scene.AddGameObject(std::move(scoreDisplay2));


		std::unique_ptr<dae::Command> moveCommand = std::make_unique<dae::MoveCommand>(player1, speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_D, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, -speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_A, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, -speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_W, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_S, dae::KeyState::Pressed);

		/*inputMan.AddController();
		std::unique_ptr<dae::Command> moveCommand = std::make_unique<dae::MoveCommand>(player1, speedObject1, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadRight, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, -speedObject1, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadLeft, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, -speedObject1);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadUp, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, speedObject1);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadDown, dae::KeyState::Pressed, 0);*/

		moveCommand = std::make_unique<dae::MoveCommand>(player2, speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadRight, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, -speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, 0, -speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadUp, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, 0, speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadDown, dae::KeyState::Pressed, 0);


		scene.AddGameObject(std::move(player1));
		scene.AddGameObject(std::move(player2));
	}

	LoadLevel("LevelsTest.txt", scene, 1);

}

void Game::LoadLevelTwo() const
{

	auto& inputMan = dae::InputCommandBinder::GetInstance();
	//inputMan.RemoveAllCommands();

	//std::unique_ptr<dae::Command> DestroyCommand = std::make_unique<dae::DestroySceneCommand>("TitleScreen");
	//DestroyCommand->Execute();

	auto& sceneManager = dae::SceneManager::GetInstance();

	auto& scene = sceneManager.CreateScene("LevelTwo");

	//Input Objects
	{
		int speedObject1 = 80;
		int speedObject2 = speedObject1 * 2;

		int starterLives = 3;



		//PLAYER 1
		auto livesDisplay = std::make_unique<dae::GameObject>(2.f, 50.f);
		livesDisplay->AddRenderComponent();
		livesDisplay->AddComponent<dae::LivesUIComponent>();
		livesDisplay->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 8);

		auto scoreDisplay = std::make_unique<dae::GameObject>(0.f, 10.f);
		scoreDisplay->AddRenderComponent();
		scoreDisplay->AddComponent<dae::ScoreUIComponent>(&dae::Achievements::GetInstance());
		scoreDisplay->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 8);

		scoreDisplay->SetParent(livesDisplay, false);

		auto player1 = std::make_unique<dae::GameObject>(38.f, 50.f);
		player1->AddRenderComponent();
		player1->AddComponent<dae::LivesComponent>(starterLives, livesDisplay->GetComponent<dae::LivesUIComponent>());
		player1->AddComponent<dae::PickUpComponent>(dae::PickUpComponent::PickUpType::Melon, scoreDisplay->GetComponent<dae::ScoreUIComponent>());
		player1->AddComponent<dae::SpriteComponent>("BubRunning.png", 4, 4, 0.1f);

		std::unique_ptr<dae::Command> killCommand = std::make_unique<dae::KillCommand>(player1);
		inputMan.AddKeyCommand(std::move(killCommand), SDL_SCANCODE_E, dae::KeyState::UpThisFrame);

		std::unique_ptr<dae::Command> pickUpCommand = std::make_unique<dae::PickUpCommand>(player1);
		inputMan.AddKeyCommand(std::move(pickUpCommand), SDL_SCANCODE_R, dae::KeyState::UpThisFrame);

		std::unique_ptr<dae::Command> pickUpCommand2 = std::make_unique<dae::PickUpCommand>(player1);
		inputMan.AddKeyCommand(std::move(pickUpCommand2), SDL_SCANCODE_T, dae::KeyState::UpThisFrame);


		scene.AddGameObject(std::move(livesDisplay));
		scene.AddGameObject(std::move(scoreDisplay));



		//PLAYER 2
		auto livesDisplay2 = std::make_unique<dae::GameObject>(2.f, 100.f);
		livesDisplay2->AddRenderComponent();
		livesDisplay2->AddComponent<dae::LivesUIComponent>();
		livesDisplay2->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 8);

		auto scoreDisplay2 = std::make_unique<dae::GameObject>(0.f, 10.f);
		scoreDisplay2->AddRenderComponent();
		scoreDisplay2->AddComponent<dae::ScoreUIComponent>(&dae::Achievements::GetInstance());
		scoreDisplay2->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 8);

		scoreDisplay2->SetParent(livesDisplay2, false);

		auto player2 = std::make_unique<dae::GameObject>(38.f, 100.f);
		player2->AddRenderComponent();
		player2->AddComponent<dae::LivesComponent>(starterLives, livesDisplay2->GetComponent<dae::LivesUIComponent>());
		player2->AddComponent<dae::PickUpComponent>(dae::PickUpComponent::PickUpType::Fries, scoreDisplay2->GetComponent<dae::ScoreUIComponent>());
		player2->AddComponent<dae::SpriteComponent>("BobRunning.png", 4, 4, 0.1f);

		inputMan.AddController();


		std::unique_ptr<dae::Command> killCommand2 = std::make_unique<dae::KillCommand>(player2);
		inputMan.AddControllerCommand(std::move(killCommand2), dae::ControllerButton::X, dae::KeyState::UpThisFrame, 0);

		pickUpCommand = std::make_unique<dae::PickUpCommand>(player2);
		inputMan.AddControllerCommand(std::move(pickUpCommand), dae::ControllerButton::A, dae::KeyState::UpThisFrame, 0);

		pickUpCommand2 = std::make_unique<dae::PickUpCommand>(player2);
		inputMan.AddControllerCommand(std::move(pickUpCommand2), dae::ControllerButton::B, dae::KeyState::UpThisFrame, 0);


		scene.AddGameObject(std::move(livesDisplay2));
		scene.AddGameObject(std::move(scoreDisplay2));


		std::unique_ptr<dae::Command> moveCommand = std::make_unique<dae::MoveCommand>(player1, speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_D, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, -speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_A, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, -speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_W, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_S, dae::KeyState::Pressed);


		moveCommand = std::make_unique<dae::MoveCommand>(player2, speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadRight, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, -speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, 0, -speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadUp, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, 0, speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadDown, dae::KeyState::Pressed, 0);


		scene.AddGameObject(std::move(player1));
		scene.AddGameObject(std::move(player2));
	}

	LoadLevel("LevelsTest.txt", scene, 2);
}
void Game::LoadLevelThree() const
{

	auto& inputMan = dae::InputCommandBinder::GetInstance();
	//inputMan.RemoveAllCommands();

	//std::unique_ptr<dae::Command> DestroyCommand = std::make_unique<dae::DestroySceneCommand>("TitleScreen");
	//DestroyCommand->Execute();

	auto& sceneManager = dae::SceneManager::GetInstance();

	auto& scene = sceneManager.CreateScene("LevelThree");

	//Input Objects
	{
		int speedObject1 = 80;
		int speedObject2 = speedObject1 * 2;

		int starterLives = 3;



		//PLAYER 1
		auto livesDisplay = std::make_unique<dae::GameObject>(2.f, 50.f);
		livesDisplay->AddRenderComponent();
		livesDisplay->AddComponent<dae::LivesUIComponent>();
		livesDisplay->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 8);

		auto scoreDisplay = std::make_unique<dae::GameObject>(0.f, 10.f);
		scoreDisplay->AddRenderComponent();
		scoreDisplay->AddComponent<dae::ScoreUIComponent>(&dae::Achievements::GetInstance());
		scoreDisplay->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 8);

		scoreDisplay->SetParent(livesDisplay, false);

		auto player1 = std::make_unique<dae::GameObject>(38.f, 50.f);
		player1->AddRenderComponent();
		player1->AddComponent<dae::LivesComponent>(starterLives, livesDisplay->GetComponent<dae::LivesUIComponent>());
		player1->AddComponent<dae::PickUpComponent>(dae::PickUpComponent::PickUpType::Melon, scoreDisplay->GetComponent<dae::ScoreUIComponent>());
		player1->AddComponent<dae::SpriteComponent>("BubRunning.png", 4, 4, 0.1f);

		std::unique_ptr<dae::Command> killCommand = std::make_unique<dae::KillCommand>(player1);
		inputMan.AddKeyCommand(std::move(killCommand), SDL_SCANCODE_E, dae::KeyState::UpThisFrame);

		std::unique_ptr<dae::Command> pickUpCommand = std::make_unique<dae::PickUpCommand>(player1);
		inputMan.AddKeyCommand(std::move(pickUpCommand), SDL_SCANCODE_R, dae::KeyState::UpThisFrame);

		std::unique_ptr<dae::Command> pickUpCommand2 = std::make_unique<dae::PickUpCommand>(player1);
		inputMan.AddKeyCommand(std::move(pickUpCommand2), SDL_SCANCODE_T, dae::KeyState::UpThisFrame);


		scene.AddGameObject(std::move(livesDisplay));
		scene.AddGameObject(std::move(scoreDisplay));



		//PLAYER 2
		auto livesDisplay2 = std::make_unique<dae::GameObject>(2.f, 100.f);
		livesDisplay2->AddRenderComponent();
		livesDisplay2->AddComponent<dae::LivesUIComponent>();
		livesDisplay2->AddComponent<dae::TextComponent>("Remaining lives: " + std::to_string(starterLives), "Lingua.otf", 8);

		auto scoreDisplay2 = std::make_unique<dae::GameObject>(0.f, 10.f);
		scoreDisplay2->AddRenderComponent();
		scoreDisplay2->AddComponent<dae::ScoreUIComponent>(&dae::Achievements::GetInstance());
		scoreDisplay2->AddComponent<dae::TextComponent>("Score: 0", "Lingua.otf", 8);

		scoreDisplay2->SetParent(livesDisplay2, false);

		auto player2 = std::make_unique<dae::GameObject>(38.f, 100.f);
		player2->AddRenderComponent();
		player2->AddComponent<dae::LivesComponent>(starterLives, livesDisplay2->GetComponent<dae::LivesUIComponent>());
		player2->AddComponent<dae::PickUpComponent>(dae::PickUpComponent::PickUpType::Fries, scoreDisplay2->GetComponent<dae::ScoreUIComponent>());
		player2->AddComponent<dae::SpriteComponent>("BobRunning.png", 4, 4, 0.1f);

		inputMan.AddController();


		std::unique_ptr<dae::Command> killCommand2 = std::make_unique<dae::KillCommand>(player2);
		inputMan.AddControllerCommand(std::move(killCommand2), dae::ControllerButton::X, dae::KeyState::UpThisFrame, 0);

		pickUpCommand = std::make_unique<dae::PickUpCommand>(player2);
		inputMan.AddControllerCommand(std::move(pickUpCommand), dae::ControllerButton::A, dae::KeyState::UpThisFrame, 0);

		pickUpCommand2 = std::make_unique<dae::PickUpCommand>(player2);
		inputMan.AddControllerCommand(std::move(pickUpCommand2), dae::ControllerButton::B, dae::KeyState::UpThisFrame, 0);


		scene.AddGameObject(std::move(livesDisplay2));
		scene.AddGameObject(std::move(scoreDisplay2));


		std::unique_ptr<dae::Command> moveCommand = std::make_unique<dae::MoveCommand>(player1, speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_D, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, -speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_A, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, -speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_W, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(player1, 0, speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_S, dae::KeyState::Pressed);


		moveCommand = std::make_unique<dae::MoveCommand>(player2, speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadRight, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, -speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, 0, -speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadUp, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(player2, 0, speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButton::DpadDown, dae::KeyState::Pressed, 0);


		scene.AddGameObject(std::move(player1));
		scene.AddGameObject(std::move(player2));
	}

	LoadLevel("LevelsTest.txt", scene, 3);
}