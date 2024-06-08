#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <memory>
#include <string>
#include <Command.h>
#include "SceneState.h"

class MainMenuState final : public SceneState
{
public:
	explicit MainMenuState() = default;
	virtual ~MainMenuState() = default;

	MainMenuState(const MainMenuState&) = delete;
	MainMenuState(MainMenuState&&) noexcept = delete;
	MainMenuState& operator= (const MainMenuState&) = delete;
	MainMenuState& operator= (MainMenuState&&) noexcept = delete;


	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;

private:
	void LoadMainMenu() const;

	static const std::string m_SceneName;
};


#endif // !MAINMENUSTATE_H