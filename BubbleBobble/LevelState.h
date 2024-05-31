#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include <memory>
#include <string>
#include <Command.h>
#include <Scene.h>
#include "SceneState.h"

class LevelState final : public SceneState
{
public:
	explicit LevelState() = default;
	virtual ~LevelState() = default;

	LevelState(const LevelState&) = delete;
	LevelState(LevelState&&) noexcept = delete;
	LevelState& operator= (const LevelState&) = delete;
	LevelState& operator= (LevelState&&) noexcept = delete;


	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;
	
	static float GetCollisionOffset() { return m_GeneralCollisionOffset; }
	static dae::GameObject* GetPlayerOne() { return m_pPlayerOne; }
	static dae::GameObject* GetPlayerTwo() { return m_pPlayerTwo; }
private:
	void MakePlayer(const std::unique_ptr<dae::GameObject>& pPlayer);

	int m_LevelNumber{1};
	static constexpr float m_GeneralCollisionOffset{3.f};

	static dae::GameObject* m_pPlayerOne;
	static dae::GameObject* m_pPlayerTwo;

	const std::string m_LevelFile{ "Levels.txt" };
	static const std::string m_SceneName;
};


#endif // !LEVELSTATE_H