#include "PauseScreenState.h"
#include <Minigin.h>
#include <GameObject.h>
#include <TextComponent.h>
#include <SceneManager.h>
#include <InputCommandBinder.h>
#include "SpriteComponent.h"
#include "Game.h"
#include "ButtonHandlerComponent.h"
#include "LoadSceneCommands.h"
#include "ActivateButtonCommand.h"
#include <KeyState.h>
#include <Renderer.h>

const std::string PauseScreenState::m_SceneName{ "Pause" };

void PauseScreenState::OnEnter()
{
	dae::Renderer::GetInstance().SetRenderBG(true);

	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);

	CreateReturnButton(scene);

	auto pGameOver = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, 24.f);
	pGameOver->AddRenderComponent(true);
	pGameOver->AddComponent<dae::TextComponent>("GAME OVER", "Fonts/Pixel_NES.otf", 24);

	scene.AddGameObject(std::move(pGameOver));

	auto pDeadBub = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 3.f, dae::Minigin::GetWindowSize().y / 2.f);
	pDeadBub->AddRenderComponent(true);
	pDeadBub->AddComponent<SpriteComponent>("Textures/BubDeath.png", 4, 4, 0.2f, true, true);

	scene.AddGameObject(std::move(pDeadBub));

	if (Game::GetInstance().GetCurrentGameMode() == Game::GameMode::MultiPlayer)
	{
		auto pDeadBob = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 3.f * 2, dae::Minigin::GetWindowSize().y / 2.f);
		pDeadBob->AddRenderComponent(true);
		pDeadBob->AddComponent<SpriteComponent>("Textures/BobDeath.png", 4, 4, 0.2f, true, true);

		scene.AddGameObject(std::move(pDeadBob));
	}
}

void PauseScreenState::OnExit()
{
	
	dae::Renderer::GetInstance().SetRenderBG(false);
}

void PauseScreenState::OnSuspend()
{
}

void PauseScreenState::OnResume()
{

}
void PauseScreenState::CreateReturnButton(dae::Scene& scene)
{
	auto pButtonHandler = std::make_unique<dae::GameObject>();
	pButtonHandler->AddComponent<ButtonHandlerComponent>();
	auto pHandlerComp = pButtonHandler->GetComponent<ButtonHandlerComponent>();

	std::unique_ptr<dae::Command> pNextLevelCommand = std::make_unique<PopSceneCommand>();
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
void PauseScreenState::LoadSinglePlayerInfo(dae::Scene&) const
{
}

void PauseScreenState::LoadMultiPlayerInfo(dae::Scene&) const
{
}

void PauseScreenState::LoadVersusInfo(dae::Scene&) const
{
}
