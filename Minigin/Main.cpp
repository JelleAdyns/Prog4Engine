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
#include "Scene.h" 
#include "Component.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "ImageComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "GameObject.h"


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	

	//Background
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::ImageComponent>("background.tga") ;

	scene.Add(go);
	  
	//DAE-Image
	go = std::make_shared<dae::GameObject>(216.f, 80.f);
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::ImageComponent>("logo.tga") ;

	scene.Add(go);

	auto gob = std::make_shared<dae::GameObject>(10.f, 10.f);
	gob->AddComponent<dae::RenderComponent>();
	gob->AddComponent<dae::ImageComponent>("logo.tga");
	scene.Add(gob);

	gob->SetParent(go, true);

	////Info
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	go = std::make_shared<dae::GameObject>(60.f, 450.f);
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("(Specify frame cap in the initialization of the Minigin)", font);
	

	scene.Add(go);


	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	
	go = std::make_shared<dae::GameObject>(60.f, 400.f);
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Press space to toggle frame cap", font);

	scene.Add(go);

	//Title
	go = std::make_shared<dae::GameObject>(80.f, 40.f);
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	
	scene.Add(go);

	//FPSCounter
	auto fpsCounter = std::make_shared<dae::GameObject>();
	fpsCounter = std::make_shared<dae::GameObject>();
	fpsCounter->AddComponent<dae::RenderComponent>();
	fpsCounter->AddComponent<dae::TextComponent>("0 FPS", font);
	fpsCounter->AddComponent<dae::FPSComponent>();
	
	scene.Add(fpsCounter);

	auto ro = std::make_shared<dae::GameObject>(20.f, 80.f);
	ro->AddComponent<dae::RenderComponent>();
	ro->AddComponent<dae::ImageComponent>("Fries.png");
	ro->AddComponent<dae::RotatorComponent>(ro->GetWorldPosition(), 50.f);

	scene.Add(ro);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/", 0.02f, 60);
	engine.Run(load);
    return 0;
}