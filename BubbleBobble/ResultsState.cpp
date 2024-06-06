#include "ResultsState.h"
#include "LevelState.h"
#include "Components.h"
#include "HighScoreHandling.h"
#include <GameObject.h>
#include <SceneManager.h>
#include <InputCommandBinder.h>
#include <AudioLocator.h>

const std::string ResultsState::m_SceneName{ "Results" };

void ResultsState::OnEnter()
{
	LoadResults();
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

void ResultsState::LoadResults() const
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);

	const auto& playerOne = LevelState::GetPlayerOne();

	unsigned int fontsize{ 10 };

	auto pEnterInitialsText = std::make_unique<dae::GameObject>(dae::Minigin::GetWindowSize().x / 2, 40);
	pEnterInitialsText->AddRenderComponent(true);
	pEnterInitialsText->AddComponent<dae::TextComponent>("Enter 1UP Initials !", "Fonts/Pixel_NES.otf", fontsize, playerOne.textColor);

	auto pScoreTag = std::make_unique<dae::GameObject>(-40, 30);
	pScoreTag->AddRenderComponent(true);
	pScoreTag->AddComponent<dae::TextComponent>("Score", "Fonts/Pixel_NES.otf", fontsize, glm::u8vec4{ 254, 241, 0, 255});
	pScoreTag->SetParent(pEnterInitialsText, false);

	auto pScore = std::make_unique<dae::GameObject>(0, 16);
	pScore->AddRenderComponent(true);
	pScore->AddComponent<dae::TextComponent>(std::to_string(playerOne.score), "Fonts/Pixel_NES.otf", fontsize);
	pScore->SetParent(pScoreTag, false);

	auto pNameTag = std::make_unique<dae::GameObject>(40, 30);
	pNameTag->AddRenderComponent(true);
	pNameTag->AddComponent<dae::TextComponent>("Name", "Fonts/Pixel_NES.otf", fontsize, glm::u8vec4{ 254, 241, 0, 255 });
	pNameTag->SetParent(pEnterInitialsText, false);

	auto pName = std::make_unique<dae::GameObject>(0, 16);
	pName->AddRenderComponent(true);
	pName->AddComponent<dae::TextComponent>(std::to_string(playerOne.score), "Fonts/Pixel_NES.otf", fontsize);
	pName->SetParent(pNameTag, false);

	highScoreHandling::WriteHighScores("One", playerOne.score);

	scene.AddGameObject(std::move(pScore));
	scene.AddGameObject(std::move(pScoreTag));
	scene.AddGameObject(std::move(pName));
	scene.AddGameObject(std::move(pNameTag));
	scene.AddGameObject(std::move(pEnterInitialsText));
}
