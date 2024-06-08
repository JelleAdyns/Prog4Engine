#ifndef LOADSCENECOMMANDS_H
#define LOADSCENECOMMANDS_H

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

class PushSceneCommand final : public dae::Command
{
public:
	PushSceneCommand(Game::CurrScene newScene) :
		dae::Command{},
		m_CurrScene{ newScene }
	{};
	virtual ~PushSceneCommand() = default;

	PushSceneCommand(const PushSceneCommand&) = delete;
	PushSceneCommand(PushSceneCommand&&) noexcept = delete;
	PushSceneCommand& operator= (const PushSceneCommand&) = delete;
	PushSceneCommand& operator= (PushSceneCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		Game::GetInstance().PushScene(m_CurrScene);
	}
private:
	const Game::CurrScene m_CurrScene;
};


class PopSceneCommand final : public dae::Command
{
public:
	PopSceneCommand() :
		dae::Command{}
	{};
	virtual ~PopSceneCommand() = default;

	PopSceneCommand(const PopSceneCommand&) = delete;
	PopSceneCommand(PopSceneCommand&&) noexcept = delete;
	PopSceneCommand& operator= (const PopSceneCommand&) = delete;
	PopSceneCommand& operator= (PopSceneCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		Game::GetInstance().PopScene();
	}

};

#endif // !LOADSCENECOMMANDS_H
