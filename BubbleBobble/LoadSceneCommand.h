#ifndef LOADSCENECOMMAND_H
#define LOADSCENECOMMAND_H

#include <Command.h>
#include "Game.h"

class LoadSceneCommand final : public dae::Command 
{
public:
	LoadSceneCommand(Game::CurrScene newScene) :
		dae::Command{},
		m_CurrScene{ newScene } 
	{};
	virtual ~LoadSceneCommand() = default;

	LoadSceneCommand(const LoadSceneCommand&) = delete;
	LoadSceneCommand(LoadSceneCommand&&) noexcept = delete;
	LoadSceneCommand& operator= (const LoadSceneCommand&) = delete;
	LoadSceneCommand& operator= (LoadSceneCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		Game::GetInstance().SetScene(m_CurrScene);
	}
private:
	const Game::CurrScene m_CurrScene;
};

#endif // !LOADSCENECOMMAND_H
