#ifndef PAUSESCREENSTATE_H
#define PAUSESCREENSTATE_H

#include <memory>
#include <string>
#include "SceneState.h"

namespace dae
{
	class Scene;
}

class PauseScreenState final : public SceneState
{
public:
	explicit PauseScreenState() = default;
	virtual ~PauseScreenState() = default;

	PauseScreenState(const PauseScreenState&) = delete;
	PauseScreenState(PauseScreenState&&) noexcept = delete;
	PauseScreenState& operator= (const PauseScreenState&) = delete;
	PauseScreenState& operator= (PauseScreenState&&) noexcept = delete;


	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;

private:
	void CreateReturnButton(dae::Scene& scene);
	void LoadSinglePlayerInfo(dae::Scene& scene) const;
	void LoadMultiPlayerInfo(dae::Scene& scene) const;
	void LoadVersusInfo(dae::Scene& scene) const;


	static const std::string m_SceneName;
};


#endif // !PAUSESCREENSTATE_H