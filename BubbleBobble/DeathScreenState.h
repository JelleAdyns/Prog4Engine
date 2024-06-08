#ifndef DEATHSCREENSTATE_H
#define DEATHSCREENSTATE_H

#include <memory>
#include <string>
#include "SceneState.h"

namespace dae
{
	class Scene;
}

class DeathScreenState final : public SceneState
{
public:
	explicit DeathScreenState() = default;
	virtual ~DeathScreenState() = default;

	DeathScreenState(const DeathScreenState&) = delete;
	DeathScreenState(DeathScreenState&&) noexcept = delete;
	DeathScreenState& operator= (const DeathScreenState&) = delete;
	DeathScreenState& operator= (DeathScreenState&&) noexcept = delete;


	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;

private:
	void LoadDeathScreen(dae::Scene& scene) const;
	void CreateButtons(dae::Scene& scene) const;

	static const std::string m_SceneName;
};


#endif // !DEATHSCREENSTATE_H