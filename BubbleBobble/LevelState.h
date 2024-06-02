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

	void AdvanceLevel();

private:
	struct Player
	{
		dae::GameObject* pPlayerObject;
		int m_Score{};
		int m_Health{};
	};

	void MakePlayer(const std::unique_ptr<dae::GameObject>& pPlayer);
	void UploadScene();
	void LoadLevel(const std::string& filename);
	void ParseLevelInfo(int levelNumber, std::ifstream& file, std::stringstream& levelInfoStream);
	void ParseStage(int levelNumber, std::stringstream& levelInfoStream);
	void ParseEnemies(std::stringstream& levelInfoStream);

	int m_LevelNumber{1};

	static constexpr float m_GeneralCollisionOffset{3.f};

	static constexpr float m_MaxLevel{3};

	dae::GameObject* m_pPlayerOne;
	dae::GameObject* m_pPlayerTwo;

	std::shared_ptr<dae::Command> m_pNextLevelCmd;

	const std::string m_LevelFile{ "Levels.txt" };
	static const std::string m_SceneName;
};


#endif // !LEVELSTATE_H