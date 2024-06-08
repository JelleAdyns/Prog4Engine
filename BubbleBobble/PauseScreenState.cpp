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
#include "SetTextCommand.h"
#include "LevelState.h"
#include <AudioLocator.h>

const std::string PauseScreenState::m_SceneName{ "Pause" };

void PauseScreenState::OnEnter()
{
	dae::Renderer::GetInstance().SetRenderBG(true);

	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);
	
	auto pause = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x/2.f, 16.f);
	pause->AddRenderComponent(true);
	pause->AddComponent<dae::TextComponent>("PAUSE", "Fonts/Pixel_NES.otf", 12);

	scene.AddGameObject(std::move(pause));

	switch (Game::GetInstance().GetCurrentGameMode())
	{
	case Game::GameMode::SinglePlayer:
		LoadSinglePlayerInfo(scene);
		break;
	case Game::GameMode::MultiPlayer:
		LoadMultiPlayerInfo(scene);
		break;
	case Game::GameMode::Versus:
		LoadVersusInfo(scene);
		break;
	}

	CreateReturnButton(scene);

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::Select), 80, false);
}

void PauseScreenState::OnExit()
{
	
	dae::Renderer::GetInstance().SetRenderBG(false);

	auto& inputMan = dae::InputCommandBinder::GetInstance();

	inputMan.RemoveChangingToControllerCommands();
	inputMan.RemoveChangingToKeyboardCommands();

	auto& audioService = dae::AudioLocator::GetAudioService();

	audioService.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::Select), 80, false);
}

void PauseScreenState::OnSuspend()
{
}

void PauseScreenState::OnResume()
{

}
void PauseScreenState::CreateReturnButton(dae::Scene& scene) const
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
void PauseScreenState::CreateLineOfText(dae::Scene& scene, const glm::vec2& pos, const std::string& keyboardText, const std::string& controllerText) const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();

	auto line = std::make_unique<dae::GameObject>(pos);
	line->AddRenderComponent(false);
	line->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? keyboardText : controllerText, "Fonts/Pixel_NES.otf", 10);

	std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(line, controllerText);
	inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
	std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(line, keyboardText);
	inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));

	scene.AddGameObject(std::move(line));
}
void PauseScreenState::LoadSinglePlayerInfo(dae::Scene& scene) const
{
	float distanceFromLeft{ 10.f };
	float distanceBetweenLines{ 16.f };
	float startHeight{ 26.f };

	auto player1 = std::make_unique<dae::GameObject>(distanceFromLeft, startHeight);
	player1->AddRenderComponent(false);
	player1->AddComponent<dae::TextComponent>("Player 1:", "Fonts/Pixel_NES.otf", 10, LevelState::GetPlayerOne().textColor);

	scene.AddGameObject(std::move(player1));

	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines }, "Move: A/D", "Move: DPAD");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 2 }, "Jump: SPACE", "Jump: A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 3 }, "Jump on bubble: hold SPACE", "Jump on bubble: hold A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 4 }, "Shoot Bubble: W", "Shoot Bubble: X");
}

void PauseScreenState::LoadMultiPlayerInfo(dae::Scene& scene) const
{
	float distanceFromLeft{ 10.f };
	float distanceBetweenLines{ 16.f };
	float startHeight{ 26.f };

	auto player1 = std::make_unique<dae::GameObject>( distanceFromLeft, startHeight );
	player1->AddRenderComponent(false);
	player1->AddComponent<dae::TextComponent>("Player 1:", "Fonts/Pixel_NES.otf", 10, LevelState::GetPlayerOne().textColor);

	scene.AddGameObject(std::move(player1));

	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines }, "Move: A/D", "Move: DPAD");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 2  }, "Jump: SPACE", "Jump: A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 3 }, "Jump on bubble: hold SPACE", "Jump on bubble: hold A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 4 }, "Shoot Bubble: W", "Shoot Bubble: X");

	auto stripes = std::make_unique<dae::GameObject>(0.f, startHeight + distanceBetweenLines * 5);
	stripes->AddRenderComponent(false);
	stripes->AddComponent<dae::TextComponent>("----------------------------------------------", "Fonts/Pixel_NES.otf", 10);

	scene.AddGameObject(std::move(stripes));

	auto player2 = std::make_unique<dae::GameObject>(distanceFromLeft, startHeight + distanceBetweenLines *6);
	player2->AddRenderComponent(false);
	player2->AddComponent<dae::TextComponent>("Player 2:", "Fonts/Pixel_NES.otf", 10, LevelState::GetPlayerTwo().textColor);

	scene.AddGameObject(std::move(player2));

	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 7}, "Move: LEFT/RIGHT", "Move: DPAD");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 8}, "Jump: UP", "Jump: A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 9}, "Jump on bubble: hold UP", "Jump on bubble: hold A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 10}, "Shoot Bubble: LCTRL", "Shoot Bubble: X");
}

void PauseScreenState::LoadVersusInfo(dae::Scene& scene) const
{
	float distanceFromLeft{ 10.f };
	float distanceBetweenLines{ 16.f };
	float startHeight{ 26.f };

	auto player1 = std::make_unique<dae::GameObject>(distanceFromLeft, startHeight);
	player1->AddRenderComponent(false);
	player1->AddComponent<dae::TextComponent>("Player 1:", "Fonts/Pixel_NES.otf", 10, LevelState::GetPlayerOne().textColor);

	scene.AddGameObject(std::move(player1));

	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines }, "Move: A/D", "Move: DPAD");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 2 }, "Jump: SPACE", "Jump: A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 3 }, "Jump on bubble: hold SPACE", "Jump on bubble: hold A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 4 }, "Shoot Bubble: W", "Shoot Bubble: X");

	auto stripes = std::make_unique<dae::GameObject>(0.f, startHeight + distanceBetweenLines * 5);
	stripes->AddRenderComponent(false);
	stripes->AddComponent<dae::TextComponent>("----------------------------------------------", "Fonts/Pixel_NES.otf", 10);

	scene.AddGameObject(std::move(stripes));

	auto player2 = std::make_unique<dae::GameObject>(distanceFromLeft, startHeight + distanceBetweenLines * 6);
	player2->AddRenderComponent(false);
	player2->AddComponent<dae::TextComponent>("Player 2:", "Fonts/Pixel_NES.otf", 10, LevelState::GetPlayerTwo().textColor);

	scene.AddGameObject(std::move(player2));

	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 7 }, "Move: LEFT/RIGHT", "Move: DPAD");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 8 }, "Jump: UP", "Jump: A");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 9 }, "Escape bubble: Tap LCTRL", "Escape bubble: Tap X");
	CreateLineOfText(scene, glm::vec2{ distanceFromLeft, startHeight + distanceBetweenLines * 10 }, "Throw Boulder: LCTRL", "Throw Boulder: X");
}
