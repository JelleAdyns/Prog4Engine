#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include <memory>
#include <string>
#include <Command.h>
#include <Scene.h>
#include <Minigin.h>
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
		LivesUIComponent* pLivesUIComp;
		int score{};
		int health{3};
		const glm::vec2 spawnPos{};
	};

	void CreateSkipButton(dae::Scene& scene);
	void MakePlayer(const std::unique_ptr<dae::GameObject>& pPlayer, PlayerComponent::PlayerType playerType, ScoreUIComponent* scoreDisplay, LivesUIComponent* livesDisplay);
	void CreateScoreDisplay(dae::Scene& scene, bool playerOne);;
	void UploadScene(dae::Scene& scene);
	void LoadLevel(const std::string& filename);
	void ParseLevelInfo(int levelNumber, std::ifstream& file, std::stringstream& levelInfoStream);
	void ParseStage(int levelNumber, std::stringstream& levelInfoStream);
	void ParseEnemies(std::stringstream& levelInfoStream);
	void CreatePlayableMaita(const std::unique_ptr<dae::GameObject>& pPlayer);

	int m_LevelNumber{1};

	Player m_pPlayerOne{ .spawnPos{24.f, dae::Minigin::GetWindowSize().y - 24.f} };
	Player m_pPlayerTwo{ .spawnPos{dae::Minigin::GetWindowSize().x - 40.f, dae::Minigin::GetWindowSize().y - 24.f} };

	const std::string m_LevelFile{ "Levels.txt" };
	static const std::string m_SceneName;
	static constexpr float m_MaxLevel{3};
	static constexpr float m_GeneralCollisionOffset{2.f};
};


#endif // !LEVELSTATE_H