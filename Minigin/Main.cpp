#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Controller.h"
#include "Scene.h" 
#include "Components.h"
#include "Commands.h"
#include "KeyState.h"
#include "GameObject.h"


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

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
	go->AddComponent<dae::ImageComponent>("background.tga") ;

	scene.Add(std::move(go));
	  
	//DAE-Image
	go = std::make_unique<dae::GameObject>(216.f, 80.f);
	go->AddRenderComponent();
	go->AddComponent<dae::ImageComponent>("logo.tga") ;


	//auto gob = std::make_unique<dae::GameObject>(10.f, 10.f);
	//gob->AddRenderComponent();
	//gob->AddComponent<dae::ImageComponent>("logo.tga");

	//gob->SetParent(go, false);
	//// REMOVING CHILD FROM PARENT
	//gob->SetParent(nullptr, true);

	scene.Add(std::move(go));
	//scene.Add(std::move(gob));


	////Info
	go = std::make_unique<dae::GameObject>(10.f, 90.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Move Bub with WASD", "Lingua.otf", 15);

	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>(10.f, 110.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Move Bob with the DPAD", "Lingua.otf", 15);

	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>(60.f, 450.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("(Specify frame cap in the initialization of the Minigin)", "Lingua.otf", 20);
	
	scene.Add(std::move(go));
	
	go = std::make_unique<dae::GameObject>(60.f, 400.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Press space to toggle frame cap", "Lingua.otf", 36);

	scene.Add(std::move(go));


	//Title
	go = std::make_unique<dae::GameObject>(80.f, 40.f);
	go->AddRenderComponent();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", "Lingua.otf", 36);
	
	scene.Add(std::move(go));

	//FPSCounter
	auto fpsCounter = std::make_unique<dae::GameObject>();
	fpsCounter->AddRenderComponent();
	fpsCounter->AddComponent<dae::TextComponent>("0 FPS", "Lingua.otf", 36);
	fpsCounter->AddComponent<dae::FPSComponent>();
	
	scene.Add(std::move(fpsCounter));

	// Rotating Sprites
	{

		auto lo = std::make_unique<dae::GameObject>(300.f, 300.f);


		auto ro = std::make_unique<dae::GameObject>();
		ro->AddRenderComponent(true);
		ro->AddComponent<dae::ImageComponent>("enemy.png");
		ro->AddComponent<dae::RotatorComponent>(75.f, 3.f);

		ro->SetParent(lo, true);

		auto so = std::make_unique<dae::GameObject>();
		so->AddRenderComponent(true);
		so->AddComponent<dae::ImageComponent>("enemy2.png");
		so->AddComponent<dae::RotatorComponent>(75.f, -5.f);

		so->SetParent(ro, true);

		scene.Add(std::move(lo));
		scene.Add(std::move(ro));

		//so->SetParent(nullptr, true);
		auto bo = std::make_unique<dae::GameObject>();
		bo->AddRenderComponent(true);
		bo->AddComponent<dae::ImageComponent>("enemy.png");
		bo->AddComponent<dae::RotatorComponent>(75.f, 8.f);

		bo->SetParent(so, false);
		scene.Add(std::move(bo));
		scene.Add(std::move(so));
	}

	//Input Objects
	{
		int speedObject1 = 150;
		int speedObject2 = speedObject1 * 2;

		auto movableObject = std::make_unique<dae::GameObject>(50.f, 150.f);
		movableObject->AddRenderComponent();
		movableObject->AddComponent<dae::ImageComponent>("Bubby.png");

		auto& inputMan = dae::InputManager::GetInstance();

		std::unique_ptr<dae::Command> moveCommand = std::make_unique<dae::MoveCommand>(movableObject, speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_D, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(movableObject, -speedObject1, 0);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_A, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(movableObject, 0, -speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_W, dae::KeyState::Pressed);
		moveCommand = std::make_unique<dae::MoveCommand>(movableObject, 0, speedObject1);
		inputMan.AddKeyCommand(std::move(moveCommand), SDL_SCANCODE_S, dae::KeyState::Pressed);


		scene.Add(std::move(movableObject));

		movableObject = std::make_unique<dae::GameObject>(50.f, 200.f);
		movableObject->AddRenderComponent();
		movableObject->AddComponent<dae::ImageComponent>("Bobby.png");

		inputMan.AddController();
		moveCommand = std::make_unique<dae::MoveCommand>(movableObject, speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadRight, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(movableObject, -speedObject2, 0);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadLeft, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(movableObject, 0, -speedObject2);
		inputMan.AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadUp, dae::KeyState::Pressed, 0);
		moveCommand = std::make_unique<dae::MoveCommand>(movableObject, 0, speedObject2);
		dae::InputManager::GetInstance().AddControllerCommand(std::move(moveCommand), dae::ControllerButtons::DpadDown, dae::KeyState::Pressed, 0);

		scene.Add(std::move(movableObject));
	}
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/", 0.02f, 60);
	engine.Run(load);
    return 0;
}