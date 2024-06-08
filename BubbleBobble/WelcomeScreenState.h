#ifndef WELCOMESCREENSTATE_H
#define WELCOMESCREENSTATE_H

#include <memory>
#include <string>
#include "SceneState.h"

class WelcomeScreenState final : public SceneState
{
public:
	explicit WelcomeScreenState() = default;
	virtual ~WelcomeScreenState() = default;

	WelcomeScreenState(const WelcomeScreenState&) = delete;
	WelcomeScreenState(WelcomeScreenState&&) noexcept = delete;
	WelcomeScreenState& operator= (const WelcomeScreenState&) = delete;
	WelcomeScreenState& operator= (WelcomeScreenState&&) noexcept = delete;


	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;

private:
	void LoadWelcomeScreen() const;

	static const std::string m_SceneName;
};
#endif // !WELCOMESCREENSTATE_H