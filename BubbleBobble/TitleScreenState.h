#ifndef TITLESCREENSTATE_H
#define TITLESCREENSTATE_H

#include <memory>
#include <string>
#include "SceneState.h"

class TitleScreenState final : public SceneState
{
public:
	explicit TitleScreenState() = default;
	virtual ~TitleScreenState() = default;

	TitleScreenState(const TitleScreenState&) = delete;
	TitleScreenState(TitleScreenState&&) noexcept = delete;
	TitleScreenState& operator= (const TitleScreenState&) = delete;
	TitleScreenState& operator= (TitleScreenState&&) noexcept = delete;


	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;

private:
	void LoadTitleScreen() const;

	static const std::string m_SceneName;
};


#endif // !TITLESCREENSTATE_H