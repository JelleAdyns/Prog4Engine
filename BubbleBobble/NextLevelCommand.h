#ifndef NEXTLEVVELCOMMAND_H
#define NEXTLEVVELCOMMAND_H

#include <Command.h>
#include "LevelState.h"

class NextLevelCommand final : public dae::Command
{
public:
	NextLevelCommand(LevelState* pLevelState) :
		dae::Command{},
		m_pLevelState{ pLevelState }
	{};
	virtual ~NextLevelCommand() = default;

	NextLevelCommand(const NextLevelCommand&) = delete;
	NextLevelCommand(NextLevelCommand&&) noexcept = delete;
	NextLevelCommand& operator= (const NextLevelCommand&) = delete;
	NextLevelCommand& operator= (NextLevelCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		m_pLevelState->AdvanceLevel();
	}
private:
	LevelState * const m_pLevelState;
};

#endif // !NEXTLEVVELCOMMAND_H
