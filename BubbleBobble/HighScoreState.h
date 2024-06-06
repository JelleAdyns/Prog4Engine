#ifndef HIGHSCORESTATE_H
#define HIGHSCORESTATE_H

#include <memory>
#include <string>
#include "SceneState.h"

class HighScoreState final : public SceneState
{
public:
	explicit HighScoreState() = default;
	virtual ~HighScoreState() = default;

	HighScoreState(const HighScoreState&) = delete;
	HighScoreState(HighScoreState&&) noexcept = delete;
	HighScoreState& operator= (const HighScoreState&) = delete;
	HighScoreState& operator= (HighScoreState&&) noexcept = delete;


	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;

private:
	void LoadHighScores() const;

	static const std::string m_SceneName;
};


#endif // !HIGHSCORESTATE_H