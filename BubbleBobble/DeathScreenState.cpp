#include "DeathScreenState.h"
#include "Components.h"
#include "Game.h"
#include <Renderer.h>
#include <GameObject.h>
#include <Minigin.h>
#include <SceneManager.h>
#include "LoadSceneCommand.h"
#include "SelectButtonCommand.h"
#include <KeyState.h>
#include <InputCommandBinder.h>
#include "ActivateButtonCommand.h"

const std::string DeathScreenState::m_SceneName{ "Death" };

void DeathScreenState::OnEnter()
{
	dae::Renderer::GetInstance().StartFadeIn(0.25f);

	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);
	LoadDeathScreen(scene);
	CreateButtons(scene);
}

void DeathScreenState::OnExit()
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();
}

void DeathScreenState::OnSuspend()
{
}

void DeathScreenState::OnResume()
{
}

void DeathScreenState::LoadDeathScreen(dae::Scene& scene) const
{
	auto pGameOver = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x /2.f, 24.f);
	pGameOver->AddRenderComponent(true);
	pGameOver->AddComponent<dae::TextComponent>("GAME OVER", "Fonts/Pixel_NES.otf", 24);

	scene.AddGameObject(std::move(pGameOver));

	auto pDeadBub = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 3.f, dae::Minigin::GetWindowSize().y /2.f);
	pDeadBub->AddRenderComponent(true);
	pDeadBub->AddComponent<SpriteComponent>("Textures/BubDeath.png", 4, 4, 0.2f, true, true);

	scene.AddGameObject(std::move(pDeadBub));

	if (Game::GetInstance().GetCurrentGameMode() == Game::GameMode::MultiPlayer)
	{
		auto pDeadBob = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 3.f * 2, dae::Minigin::GetWindowSize().y /2.f);
		pDeadBob->AddRenderComponent(true);
		pDeadBob->AddComponent<SpriteComponent>("Textures/BobDeath.png", 4, 4, 0.2f, true, true);

		scene.AddGameObject(std::move(pDeadBob));
	}
}

void DeathScreenState::CreateButtons(dae::Scene& scene) const
{
	auto buttonHandler = std::make_unique<dae::GameObject>();
	buttonHandler->AddComponent<ButtonHandlerComponent>();

	const auto& handlerComponent = buttonHandler->GetComponent<ButtonHandlerComponent>();

	const float distanceBetweenButton{ 20.f };
	std::unique_ptr<dae::Command> loadCommand = std::make_unique<LoadSceneCommand>(Game::CurrScene::Results);

	auto button1 = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, dae::Minigin::GetWindowSize().y / 4.f*3);
	button1->AddRenderComponent(true);
	button1->AddComponent<dae::TextComponent>("Enter Score", "Fonts/Pixel_NES.otf", 10);
	button1->AddComponent<ButtonComponent>(std::move(loadCommand));
	
	handlerComponent->AddButton(button1->GetComponent<ButtonComponent>());

	std::unique_ptr<dae::Command> loadCommand2 = std::make_unique<LoadSceneCommand>(Game::CurrScene::TitleScreen);

	auto button2 = std::make_unique<dae::GameObject>(0.f, distanceBetweenButton);
	button2->AddRenderComponent(true);
	button2->AddComponent<dae::TextComponent>("Title Screen", "Fonts/Pixel_NES.otf", 10);
	button2->AddComponent<ButtonComponent>(std::move(loadCommand2));
	button2->SetParent(button1, false);

	handlerComponent->AddButton(button2->GetComponent<ButtonComponent>());

	
	float iconOffset{60};

	auto icon = std::make_unique<dae::GameObject>(button1->GetWorldPosition().x - iconOffset, button1->GetWorldPosition().y);
	icon->AddRenderComponent(true);
	icon->AddComponent<SpriteComponent>("Textures/Selector.png", 2, 1, 0.1f);


	auto& inputMan = dae::InputCommandBinder::GetInstance();
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
	scene.AddGameObject(std::move(button1));
	scene.AddGameObject(std::move(button2));
	scene.AddGameObject(std::move(icon));
}
