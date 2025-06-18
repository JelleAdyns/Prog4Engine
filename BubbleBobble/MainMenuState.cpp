#include "MainMenuState.h"
#include <AudioLocator.h>
#include <SDLAudio.h>
#include <LogAudio.h>
#include <SceneManager.h>
#include <Renderer.h>
#include <InputCommandBinder.h>
#include <Minigin.h>
#include "Components.h"
#include "LoadSceneCommands.h"
#include "Game.h"
#include "SelectButtonCommand.h"
#include "StartLevelCommand.h"
#include <KeyState.h>
#include "ActivateButtonCommand.h"
#include "HighScoreState.h"
#include "TitleScreenState.h"
#include "SetTextCommand.h"

const std::string MainMenuState::m_SceneName{ "MainMenu" };


void MainMenuState::OnEnter()
{
	dae::Renderer::GetInstance().StartFadeIn(0.5f);
	LoadMainMenu();
}

void MainMenuState::OnExit()
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.StopAllSounds();

	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::Select), 80, false);

	switch (Game::GetInstance().GetCurrentGameMode())
	{
	case Game::GameMode::SinglePlayer:
		break;
	case Game::GameMode::MultiPlayer:
		dae::InputCommandBinder::GetInstance().AddController();
		break;
	case Game::GameMode::Versus:
		dae::InputCommandBinder::GetInstance().AddController();
		break;

	}
}

void MainMenuState::OnSuspend()
{
	
}

void MainMenuState::OnResume()
{
	
}

void MainMenuState::LoadMainMenu() const
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& scene = sceneManager.CreateScene(m_SceneName);

	auto& inputMan = dae::InputCommandBinder::GetInstance();

	auto bubbleBobbleText = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2 - 30.f, 20.f);
	bubbleBobbleText->AddRenderComponent(false);
	bubbleBobbleText->AddComponent<dae::TextComponent>("Bubble Bobble", "Fonts/Pixel_NES.otf", 10);


	auto buttonHandler = std::make_unique<dae::GameObject>();
	buttonHandler->AddComponent<ButtonHandlerComponent>();


	const auto& handlerComponent = buttonHandler->GetComponent<ButtonHandlerComponent>();

	const float distanceBetweenButton{ 20.f };
	std::unique_ptr<dae::Command> loadCommand = std::make_unique<StartLevelCommand>(Game::GameMode::SinglePlayer);

	auto button1 = std::make_unique<dae::GameObject>(0.f, distanceBetweenButton);
	button1->AddRenderComponent(false);
	button1->AddComponent<dae::TextComponent>("1P Start", "Fonts/Pixel_NES.otf", 10);
	button1->AddComponent<ButtonComponent>(std::move(loadCommand));
	button1->SetParent(bubbleBobbleText, false);

	handlerComponent->AddButton(button1->GetComponent<ButtonComponent>());


	std::unique_ptr<dae::Command> loadCommand2 = std::make_unique<StartLevelCommand>(Game::GameMode::MultiPlayer);

	auto button2 = std::make_unique<dae::GameObject>(0.f, distanceBetweenButton);
	button2->AddRenderComponent(false);
	button2->AddComponent<dae::TextComponent>("2P Start", "Fonts/Pixel_NES.otf", 10);
	button2->AddComponent<ButtonComponent>(std::move(loadCommand2));
	button2->SetParent(button1, false);

	handlerComponent->AddButton(button2->GetComponent<ButtonComponent>());

	std::unique_ptr<dae::Command> loadCommand3 = std::make_unique<StartLevelCommand>(Game::GameMode::Versus);

	auto button3 = std::make_unique<dae::GameObject>(0.f, distanceBetweenButton);
	button3->AddRenderComponent(false);
	button3->AddComponent<dae::TextComponent>("Versus", "Fonts/Pixel_NES.otf", 10);
	button3->AddComponent<ButtonComponent>(std::move(loadCommand3));
	button3->SetParent(button2, false);

	handlerComponent->AddButton(button3->GetComponent<ButtonComponent>());


	std::unique_ptr<dae::Command> loadCommand4 = std::make_unique<LoadSceneCommand<HighScoreState>>();

	auto button4 = std::make_unique<dae::GameObject>(0.f, distanceBetweenButton);
	button4->AddRenderComponent(false);
	button4->AddComponent<dae::TextComponent>("HighScores", "Fonts/Pixel_NES.otf", 10);
	button4->AddComponent<ButtonComponent>(std::move(loadCommand4));
	button4->SetParent(button3, false);

	handlerComponent->AddButton(button4->GetComponent<ButtonComponent>());


	std::unique_ptr<dae::Command> loadCommand5 = std::make_unique<LoadSceneCommand<TitleScreenState>>();

	auto button5 = std::make_unique<dae::GameObject>(0.f, distanceBetweenButton);
	button5->AddRenderComponent(false);
	button5->AddComponent<dae::TextComponent>("TitleScreen", "Fonts/Pixel_NES.otf", 10);
	button5->AddComponent<ButtonComponent>(std::move(loadCommand5));
	button5->SetParent(button4, false);

	handlerComponent->AddButton(button5->GetComponent<ButtonComponent>());


	float iconOffset{20.f};

	auto icon = std::make_unique<dae::GameObject>(button1->GetWorldPosition().x - iconOffset, button1->GetWorldPosition().y);
	icon->AddRenderComponent(false);
	icon->AddComponent<SpriteComponent>("Textures/Selector.png", 2, 1, 0.1f);


	std::shared_ptr<dae::Command> nextCommand = std::make_shared<SelectButtonCommand>(buttonHandler, icon, iconOffset, SelectButtonCommand::Direction::Next);
	inputMan.AddKeyCommand(nextCommand, SDL_SCANCODE_S, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(nextCommand, dae::ControllerButton::DpadDown, dae::KeyState::DownThisFrame, 0);

	std::shared_ptr<dae::Command> previousCommand = std::make_shared<SelectButtonCommand>(buttonHandler, icon, iconOffset, SelectButtonCommand::Direction::Previous);
	inputMan.AddKeyCommand(previousCommand, SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(previousCommand, dae::ControllerButton::DpadUp, dae::KeyState::DownThisFrame, 0);

	std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(buttonHandler);
	inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_RETURN, dae::KeyState::UpThisFrame);
	inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::A, dae::KeyState::UpThisFrame, 0);

	scene.AddGameObject(std::move(buttonHandler));
	scene.AddGameObject(std::move(bubbleBobbleText));
	scene.AddGameObject(std::move(icon));
	scene.AddGameObject(std::move(button1));
	scene.AddGameObject(std::move(button2));
	scene.AddGameObject(std::move(button3));
	scene.AddGameObject(std::move(button4));
	scene.AddGameObject(std::move(button5));

	auto info = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, 180.f);
	info->AddRenderComponent(true);
	info->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? "Move: W/S | choose: ENTER" : "Move: DPAD | choose: A", "Fonts/Pixel_NES.otf", 10);

	std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(info, "Move: DPAD | choose: A");
	inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
	std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(info, "Move: W/S | choose: ENTER");
	inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));

	scene.AddGameObject(std::move(info));
}
