#include "WelcomeScreenState.h"
#include <Renderer.h>
#include <InputCommandBinder.h>
#include <SceneManager.h>
#include <Minigin.h>
#include <TextComponent.h>
#include "SpriteComponent.h"
#include "RotatorComponent.h"
#include "Game.h"
#include "TimerComponent.h"
#include "LoadSceneCommands.h"
#include "LevelState.h"
#include <AudioLocator.h>

const std::string WelcomeScreenState::m_SceneName{ "WelcomeScreen" };

void WelcomeScreenState::OnEnter()
{
	dae::Renderer::GetInstance().StartFadeIn(0.5f);

	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	LoadWelcomeScreen();

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.AddSound("Sounds/MainTheme.mp3", static_cast<dae::SoundID>(Game::SoundEvent::MainTheme));
	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::MainTheme), 80, true);
}

void WelcomeScreenState::OnExit()
{
}

void WelcomeScreenState::OnSuspend()
{
}

void WelcomeScreenState::OnResume()
{
}

void WelcomeScreenState::LoadWelcomeScreen() const
{
	const glm::u8vec4 red{ 255,0,0,255 };
	float distanceBetweenLines{ 16.f };
	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);

	auto pNow = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, distanceBetweenLines);
	pNow->AddRenderComponent(true);
	pNow->AddComponent<dae::TextComponent>("Now, it is beginning of a ", "Fonts/Pixel_NES.otf", 10, red);

	scene.AddGameObject(std::move(pNow));

	auto pFantasticStory = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, distanceBetweenLines*2);
	pFantasticStory->AddRenderComponent(true);
	pFantasticStory->AddComponent<dae::TextComponent>("fantastic story!! Let's make a ", "Fonts/Pixel_NES.otf", 10, red);

	scene.AddGameObject(std::move(pFantasticStory));

	auto pJourney = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, distanceBetweenLines*3);
	pJourney->AddRenderComponent(true);
	pJourney->AddComponent<dae::TextComponent>("journey to the cave of monsters!", "Fonts/Pixel_NES.otf", 10, red);

	scene.AddGameObject(std::move(pJourney));

	auto pGoodLuck = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, distanceBetweenLines*4);
	pGoodLuck->AddRenderComponent(true);
	pGoodLuck->AddComponent<dae::TextComponent>("Good luck!", "Fonts/Pixel_NES.otf", 10, red);

	scene.AddGameObject(std::move(pGoodLuck));


	auto pBubParent = std::make_unique<dae::GameObject>(70.f, dae::Minigin::GetWindowSize().y / 2.f+20);
	auto pBub = std::make_unique<dae::GameObject>();
	pBub->AddRenderComponent(true);
	pBub->AddComponent<SpriteComponent>("Textures/BubIntro.png",2,1,0.1f,true,true);
	pBub->AddComponent<RotatorComponent>(20.f,0.f, 2.f);
	pBub->SetParent(pBubParent, false);

	scene.AddGameObject(std::move(pBubParent));
	scene.AddGameObject(std::move(pBub));

	if(Game::GetInstance().GetCurrentGameMode() == Game::GameMode::MultiPlayer)
	{
		auto pBobParent = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x -70.f, dae::Minigin::GetWindowSize().y / 2.f+20);
		auto pBob = std::make_unique<dae::GameObject>();
		pBob->AddRenderComponent(true);
		pBob->AddComponent<SpriteComponent>("Textures/BobIntro.png", 2, 1, 0.1f, true, true);
		pBob->AddComponent<RotatorComponent>(20.f,180.f, -2.f);
		pBob->SetParent(pBobParent, false);

		scene.AddGameObject(std::move(pBobParent));
		scene.AddGameObject(std::move(pBob));
	}

	std::unique_ptr<dae::Command> nextScene = std::make_unique<LoadSceneCommand<LevelState>>();

	auto pTimer = std::make_unique<dae::GameObject>();
	pTimer->AddComponent<TimerComponent>(8.f, std::move(nextScene));
	pTimer->GetComponent<TimerComponent>()->Activate();

	scene.AddGameObject(std::move(pTimer));
}
