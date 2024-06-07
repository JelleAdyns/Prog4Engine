#include "ResultsState.h"
#include "LevelState.h"
#include "Components.h"
#include "HighScoreHandling.h"
#include "LoadSceneCommand.h"
#include "InitialsCommands.h"
#include <GameObject.h>
#include <SceneManager.h>
#include <InputCommandBinder.h>
#include <Renderer.h>
#include <AudioLocator.h>
#include <KeyState.h>
#include "ActivateButtonCommand.h"
#include "SetTextCommand.h"

const std::string ResultsState::m_SceneName{ "Results" };
const std::string ResultsState::m_TemporaryInitials{ "   " };

ResultsState::~ResultsState()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		if (pSubject) pSubject->RemoveObserver(this);
	}
}

void ResultsState::OnEnter()
{
	dae::Renderer::GetInstance().StartFadeIn();

	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);
	LoadPlayerScore(scene, m_TemporaryInitials);
	LoadTopFive(scene);
	LoadInputInfo(scene);
	
}

void ResultsState::OnExit()
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.StopAllSounds();
}

void ResultsState::OnSuspend()
{
}

void ResultsState::OnResume()
{
}

void ResultsState::Notify(InitialsComponent* pSubject)
{
	++m_RefreshCount;

	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName+std::to_string(m_RefreshCount));

	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	if(Game::GetInstance().GetCurrentGameMode() == Game::GameMode::MultiPlayer)
	{
		if (m_RefreshCount == 2) m_ReadyToLeave = true;
	}
	else if (m_RefreshCount == 1) m_ReadyToLeave = true;

	if(m_ReadyToLeave)
	{
		auto buttonHandler = std::make_unique<dae::GameObject>();
		buttonHandler->AddComponent<ButtonHandlerComponent>();

		std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(buttonHandler);
		inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_SPACE, dae::KeyState::DownThisFrame);
		inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::X, dae::KeyState::DownThisFrame, 0);

		const auto& handlerComponent = buttonHandler->GetComponent<ButtonHandlerComponent>();

		std::unique_ptr<dae::Command> pNextSceneCmd = std::make_unique<LoadSceneCommand>(Game::CurrScene::TitleScreen);
		auto button = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, 180.f);
		button->AddComponent<ButtonComponent>(std::move(pNextSceneCmd));

		handlerComponent->AddButton(button->GetComponent<ButtonComponent>());
		scene.AddGameObject(std::move(buttonHandler));
		scene.AddGameObject(std::move(button));
	}

	highScoreHandling::RemoveHighScore(m_TemporaryInitials);
	std::string initials{};
	pSubject->GetInitials(initials);
	highScoreHandling::WriteHighScores(initials, m_RefreshCount < 2 ? LevelState::GetPlayerOne().score : LevelState::GetPlayerTwo().score);

	LoadPlayerScore(scene, initials);
	LoadTopFive(scene);
	LoadInputInfo(scene);
	
}

void ResultsState::AddSubjectPointer(dae::Subject<InitialsComponent>* pSubject)
{
	m_pVecObservedSubjects.emplace_back(pSubject);
}

void ResultsState::SetSubjectPointersInvalid(dae::Subject<InitialsComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pSubject);
	if (pos != m_pVecObservedSubjects.cend())
	{
		m_pVecObservedSubjects.erase(pos);
	}
}

void ResultsState::LoadPlayerScore(dae::Scene& scene, const std::string& name)
{
	auto gameMode = Game::GetInstance().GetCurrentGameMode();

	const auto& player = (m_RefreshCount > 0 && gameMode == Game::GameMode::MultiPlayer) ? LevelState::GetPlayerTwo() : LevelState::GetPlayerOne();

	auto pEnterInitialsText = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2, 32);
	pEnterInitialsText->AddRenderComponent(true);
	pEnterInitialsText->AddComponent<dae::TextComponent>(
		(m_RefreshCount > 0 && gameMode == Game::GameMode::MultiPlayer) ? "Enter 2UP Initials !" : "Enter 1UP Initials !",
		highScoreHandling::fontName,
		highScoreHandling::fontsize,
		player.textColor);

	auto pScoreTag = std::make_unique<dae::GameObject>(-40, 24);
	pScoreTag->AddRenderComponent(true);
	pScoreTag->AddComponent<dae::TextComponent>("Score", highScoreHandling::fontName, highScoreHandling::fontsize, m_Yellow);
	pScoreTag->SetParent(pEnterInitialsText, false);

	auto pScore = std::make_unique<dae::GameObject>(0, 16);
	pScore->AddRenderComponent(true);
	pScore->AddComponent<dae::TextComponent>(std::to_string(player.score), highScoreHandling::fontName, highScoreHandling::fontsize);
	pScore->SetParent(pScoreTag, false);

	auto pNameTag = std::make_unique<dae::GameObject>(40, 24);
	pNameTag->AddRenderComponent(true);
	pNameTag->AddComponent<dae::TextComponent>("Name", highScoreHandling::fontName, highScoreHandling::fontsize, m_Yellow);
	pNameTag->SetParent(pEnterInitialsText, false);

	auto pName = std::make_unique<dae::GameObject>(0, 16);
	pName->AddRenderComponent(true);
	pName->AddComponent<dae::TextComponent>(name, highScoreHandling::fontName, highScoreHandling::fontsize);
	pName->SetParent(pNameTag, false);

	if(!m_ReadyToLeave)
	{
		pName->AddComponent<InitialsComponent>(this);

		auto& inputMan = dae::InputCommandBinder::GetInstance();
		std::shared_ptr<dae::Command> pNextCharacterCmd = std::make_shared<AdvanceCharacterCommand>(pName.get(), true);
		std::shared_ptr<dae::Command> pPreviousCharacterCmd = std::make_shared<AdvanceCharacterCommand>(pName.get(), false);
		std::shared_ptr<dae::Command> pConfirmCharacterCmd = std::make_shared<ConfirmCharacterCommand>(pName.get());

		if (m_RefreshCount == 0)
		{
			inputMan.AddKeyCommand(pNextCharacterCmd, SDL_SCANCODE_D, dae::KeyState::DownThisFrame);
			inputMan.AddControllerCommand(pNextCharacterCmd, dae::ControllerButton::DpadRight, dae::KeyState::DownThisFrame, 0);

			inputMan.AddKeyCommand(pPreviousCharacterCmd, SDL_SCANCODE_A, dae::KeyState::DownThisFrame);
			inputMan.AddControllerCommand(pPreviousCharacterCmd, dae::ControllerButton::DpadLeft, dae::KeyState::DownThisFrame, 0);

			inputMan.AddKeyCommand(pConfirmCharacterCmd, SDL_SCANCODE_W, dae::KeyState::DownThisFrame);
			inputMan.AddControllerCommand(pConfirmCharacterCmd, dae::ControllerButton::A, dae::KeyState::DownThisFrame, 0);
		}
		else if (m_RefreshCount == 1)
		{
			inputMan.AddKeyCommand(pNextCharacterCmd, SDL_SCANCODE_RIGHT, dae::KeyState::DownThisFrame);
			inputMan.AddControllerCommand(pNextCharacterCmd, dae::ControllerButton::DpadRight, dae::KeyState::DownThisFrame, 1);

			inputMan.AddKeyCommand(pPreviousCharacterCmd, SDL_SCANCODE_LEFT, dae::KeyState::DownThisFrame);
			inputMan.AddControllerCommand(pPreviousCharacterCmd, dae::ControllerButton::DpadLeft, dae::KeyState::DownThisFrame, 1);

			inputMan.AddKeyCommand(pConfirmCharacterCmd, SDL_SCANCODE_UP, dae::KeyState::DownThisFrame);
			inputMan.AddControllerCommand(pConfirmCharacterCmd, dae::ControllerButton::A, dae::KeyState::DownThisFrame, 1);
		}

		highScoreHandling::WriteHighScores(m_TemporaryInitials, player.score);
	}


	scene.AddGameObject(std::move(pScore));
	scene.AddGameObject(std::move(pScoreTag));
	scene.AddGameObject(std::move(pName));
	scene.AddGameObject(std::move(pNameTag));
	scene.AddGameObject(std::move(pEnterInitialsText));
}

void ResultsState::LoadTopFive(dae::Scene& scene) const
{
	float startHeight{ dae::Minigin::GetWindowSize().y / 2.f - 16 };

	auto pScoreTag = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, startHeight);
	pScoreTag->AddRenderComponent(true);
	pScoreTag->AddComponent<dae::TextComponent>("Score", highScoreHandling::fontName, highScoreHandling::fontsize, m_Yellow);

	auto pNameTag = std::make_unique<dae::GameObject>(40, 0);
	pNameTag->AddRenderComponent(true);
	pNameTag->AddComponent<dae::TextComponent>("Name", highScoreHandling::fontName, highScoreHandling::fontsize, m_Yellow);
	pNameTag->SetParent(pScoreTag, false);

	float placeBetweenLines{ 16.f };
	std::vector<highScoreHandling::PlayerScore> highscores{};
	highScoreHandling::LoadHighScores(highscores);
	
	if (highscores.size() > 5) highscores.erase(highscores.begin() + 5, highscores.end());

	for (size_t i = 1; i <= highscores.size(); i++)
	{
		glm::vec2 pos{ dae::Minigin::GetWindowSize().x / 2.f - 40.f, startHeight + i * placeBetweenLines };
		highScoreHandling::CreateScoreLineObjects(scene, pos, highscores.at(i-1), static_cast<int>(i));
	}


	scene.AddGameObject(std::move(pScoreTag));
	scene.AddGameObject(std::move(pNameTag));
}

void ResultsState::LoadInputInfo(dae::Scene& scene) const
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	auto info = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, dae::Minigin::GetWindowSize().y - 16.f);
	info->AddRenderComponent(true);
	if(m_ReadyToLeave)
	{
		info->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? "Continue: SPACE" : "Continue: X", "Fonts/Pixel_NES.otf", 8);

		std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(info, "Continue: X");
		inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
		std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(info, "Continue: SPACE");
		inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));
	}
	else
	{
		std::string keyboardText{};
		std::string controllerText{ "Cycle: DPAD | Confirm: A" };
		if (m_RefreshCount == 0) keyboardText = "Cycle: A/D | Confirm: W";
		else if (m_RefreshCount == 1) keyboardText = "Cycle: Left/Right | Confirm: Up";

		info->AddComponent<dae::TextComponent>(inputMan.IsKeyboardActive() ? keyboardText : controllerText, "Fonts/Pixel_NES.otf", 8);

		std::unique_ptr<dae::Command> setTextCommand = std::make_unique<SetTextCommand>(info, controllerText);
		inputMan.AddCommand_ChangingToController(std::move(setTextCommand));
		std::unique_ptr<dae::Command> setTextCommand2 = std::make_unique<SetTextCommand>(info, keyboardText);
		inputMan.AddCommand_ChangingToKeyboard(std::move(setTextCommand2));
	}
	scene.AddGameObject(std::move(info));
}


