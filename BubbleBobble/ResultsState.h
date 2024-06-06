#ifndef RESULTSSTATE_H
#define RESULTSSTATE_H

#include <memory>
#include <string>
#include "SceneState.h"

class ResultsState final : public SceneState
{
public:
	explicit ResultsState() = default;
	virtual ~ResultsState() = default;

	ResultsState(const ResultsState&) = delete;
	ResultsState(ResultsState&&) noexcept = delete;
	ResultsState& operator= (const ResultsState&) = delete;
	ResultsState& operator= (ResultsState&&) noexcept = delete;


	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;

private:
	void LoadResults() const;

	static const std::string m_SceneName;
};


#endif // !RESULTSSTATE_H