#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include <memory>
#include <string>
#include <Command.h>
#include <Scene.h>
#include "SceneState.h"
#include "PlayerComponent.h"

class ScoreUIComponent;
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
		ScoreUIComponent* pScoreUIComp;
		int score{};
		int health{};
	};

	void CreateSkipButton(dae::Scene& scene);
	void MakePlayer(const std::unique_ptr<dae::GameObject>& pPlayer, PlayerComponent::PlayerType playerType, ScoreUIComponent* scoreDisplay, LivesUIComponent* livesDisplay);
	void CreateScoreDisplay(dae::Scene& scene, bool playerOne);
	void CreateLivesDisplay();
	void UploadScene(dae::Scene& scene);
	void LoadLevel(const std::string& filename);
	void ParseLevelInfo(int levelNumber, std::ifstream& file, std::stringstream& levelInfoStream);
	void ParseStage(int levelNumber, std::stringstream& levelInfoStream);
	void ParseEnemies(std::stringstream& levelInfoStream);


	int m_LevelNumber{1};

	Player m_pPlayerOne{};
	Player m_pPlayerTwo;

	const std::string m_LevelFile{ "Levels.txt" };
	static const std::string m_SceneName;
	static constexpr float m_MaxLevel{3};
	static constexpr float m_GeneralCollisionOffset{3.f};
};


#endif // !LEVELSTATE_H