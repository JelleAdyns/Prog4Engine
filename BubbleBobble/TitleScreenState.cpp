#include "TitleScreenState.h"
#include <AudioLocator.h>
#include <SDLAudio.h>
#include <LogAudio.h>
#include <SceneManager.h>
#include <InputCommandBinder.h>
#include <Minigin.h>
#include <Renderer.h>
#include "Components.h"
#include "LoadSceneCommand.h"
#include "Game.h"
#include "ActivateButtonCommand.h"
#include <KeyState.h>
#include "SetTextCommand.h"

const std::string TitleScreenState::m_SceneName{ "TitleScreen" };

void TitleScreenState::OnEnter()
{
	dae::Renderer::GetInstance().StartFadeIn(0.5f);
	
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();
	


	LoadTitleScreen();

	//dae::SceneManager::GetInstance().RemoveNonActiveScenes();

	//dae::SceneManager::GetInstance().Start();
	
}

void TitleScreenState::OnExit()
{
	//dae::Renderer::GetInstance().StartFadeOut();
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.StopAllSounds();

	audioService.AddSound("Sounds/Select.wav", static_cast<dae::SoundID>(Game::SoundEvent::Select));
	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::Select), 80, false);

}

void TitleScreenState::OnSuspend()
{
}

void TitleScreenState::OnResume()
{
}


void TitleScreenState::LoadTitleScreen() const
{

	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& scene = sceneManager.CreateScene(m_SceneName);
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	auto titleLogo = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, static_cast<float>(dae::Minigin::GetWindowSize().y) / 2 - 20);
	titleLogo->AddRenderComponent(true);
	titleLogo->AddComponent<SpriteComponent>("Textures/TitleLogo.png", 6, 1, 0.2f);

	scene.AddGameObject(std::move(titleLogo));


	auto buttonHandler = std::make_unique<dae::GameObject>();
	buttonHandler->AddComponent<ButtonHandlerComponent>();

	std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(buttonHandler);
	inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_SPACE, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::Start, dae::KeyState::DownThisFrame, 0);

	const auto& handlerComponent = buttonHandler->GetComponent<ButtonHandlerComponent>();

	std::unique_ptr<dae::Command> loadCommand = std::make_unique<LoadSceneCommand>(Game::CurrScene::Menu);

	auto button1 = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, 180.f);
	button1->AddRenderComponent(true);
	button1->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? "Press Space" : "Push Start Button", "Fonts/Pixel_NES.otf", 10);
	button1->AddComponent<ButtonComponent>(std::move(loadCommand));


	handlerComponent->AddButton(button1->GetComponent<ButtonComponent>());

	std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(button1, "Push Start Button");
	inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
	std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(button1, "Press Space");
	inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));


	auto credit = std::make_unique<dae::GameObject>(3.f, dae::Minigin::GetWindowSize().y - 8.f);
	credit->AddRenderComponent(false);
	credit->AddComponent<dae::TextComponent>("Made by Jelle Adyns", "Fonts/Pixel_NES.otf", 5);

	scene.AddGameObject(std::move(buttonHandler));
	scene.AddGameObject(std::move(button1));
	scene.AddGameObject(std::move(credit));
}

