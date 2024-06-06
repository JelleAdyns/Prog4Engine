#include "HighScoreState.h"
#include <InputCommandBinder.h>
#include <AudioLocator.h>

void HighScoreState::OnEnter()
{
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

void HighScoreState::LoadHighScores() const
{
}
