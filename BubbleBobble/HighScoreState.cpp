#include "HighScoreState.h"
#include "HighScoreHandling.h"
#include "TextComponent.h"
#include <InputCommandBinder.h>
#include <AudioLocator.h>
#include <SceneManager.h>
#include <Renderer.h>
#include <Minigin.h>
#include "ButtonHandlerComponent.h"
#include "ActivateButtonCommand.h"
#include <KeyState.h>
#include "LoadSceneCommands.h"

const std::string HighScoreState::m_SceneName{ "HighScore" };

void HighScoreState::OnEnter()
{
	dae::Renderer::GetInstance().StartFadeIn(0.5f);

	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);

	auto& inputMan = dae::InputCommandBinder::GetInstance();
	//inputMan.RemoveAllCommands();

	auto buttonHandler = std::make_unique<dae::GameObject>();
	buttonHandler->AddComponent<ButtonHandlerComponent>();

	std::shared_ptr<dae::Command> activateCommand = std::make_shared<ActivateButtonCommand>(buttonHandler);
	inputMan.AddKeyCommand(activateCommand, SDL_SCANCODE_ESCAPE, dae::KeyState::DownThisFrame);
	inputMan.AddControllerCommand(activateCommand, dae::ControllerButton::B, dae::KeyState::DownThisFrame, 0);

	const auto& handlerComponent = buttonHandler->GetComponent<ButtonHandlerComponent>();

	std::unique_ptr<dae::Command> pNextSceneCmd = std::make_unique<LoadSceneCommand>(Game::CurrScene::Menu);
	auto button = std::make_unique<dae::GameObject>(static_cast<float>(dae::Minigin::GetWindowSize().x) / 2, 180.f);
	button->AddComponent<ButtonComponent>(std::move(pNextSceneCmd));

	handlerComponent->AddButton(button->GetComponent<ButtonComponent>());

	scene.AddGameObject(std::move(buttonHandler));
	scene.AddGameObject(std::move(button));

	LoadHighScores(scene);
}

void HighScoreState::OnExit()
{
	auto& inputMan = dae::InputCommandBinder::GetInstance();
	inputMan.RemoveAllCommands();

	auto& audioService = dae::AudioLocator::GetAudioService();
	audioService.StopAllSounds();
}

void HighScoreState::OnSuspend()
{
}

void HighScoreState::OnResume()
{
}

void HighScoreState::LoadHighScores(dae::Scene& scene) const
{


	auto pTitle = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, 16.f);
	pTitle->AddRenderComponent(true);
	pTitle->AddComponent<dae::TextComponent>("HighScores", highScoreHandling::fontName, highScoreHandling::fontsize * 2, glm::u8vec4{255,0,0,255});

	float startHeight{ 40 };

	auto pScoreTag = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2.f, startHeight);
	pScoreTag->AddRenderComponent(true);
	pScoreTag->AddComponent<dae::TextComponent>("Score", highScoreHandling::fontName, highScoreHandling::fontsize, glm::u8vec4{ 254, 241, 0, 255 });

	auto pNameTag = std::make_unique<dae::GameObject>(40, 0);
	pNameTag->AddRenderComponent(true);
	pNameTag->AddComponent<dae::TextComponent>("Name", highScoreHandling::fontName, highScoreHandling::fontsize, glm::u8vec4{ 254, 241, 0, 255 });
	pNameTag->SetParent(pScoreTag, false);

	float placeBetweenLines{ 16.f };
	std::vector<highScoreHandling::PlayerScore> highscores{};
	highScoreHandling::LoadHighScores(highscores);

	if (highscores.size() > 10) highscores.erase(highscores.begin() + 10, highscores.end());

	for (size_t i = 1; i <= highscores.size(); i++)
	{
		glm::vec2 pos{ dae::Minigin::GetWindowSize().x / 2.f - 40.f, startHeight + i * placeBetweenLines };
		highScoreHandling::CreateScoreLineObjects(scene, pos, highscores.at(i - 1), static_cast<int>(i));
	}

	scene.AddGameObject(std::move(pTitle));
	scene.AddGameObject(std::move(pScoreTag));
	scene.AddGameObject(std::move(pNameTag));
}
