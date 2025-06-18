#ifndef STARTLEVELCOMMAND_H
#define STARTLEVELCOMMAND_H

#include <Command.h>
#include "Game.h"
#include "WelcomeScreenState.h"

class StartLevelCommand final : public dae::Command
{
public:
	StartLevelCommand(Game::GameMode newGameMode) :
		dae::Command{},
		m_GameMode{ newGameMode }
	{};
	virtual ~StartLevelCommand() = default;

	StartLevelCommand(const StartLevelCommand&) = delete;
	StartLevelCommand(StartLevelCommand&&) noexcept = delete;
	StartLevelCommand& operator= (const StartLevelCommand&) = delete;
	StartLevelCommand& operator= (StartLevelCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		Game::GetInstance().SetGameMode(m_GameMode);
		Game::GetInstance().SetScene<WelcomeScreenState>();
	}
private:
	const Game::GameMode m_GameMode;
};

#endif // !STARTLEVELCOMMAND_H
