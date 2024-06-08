#ifndef RESULTSSTATE_H
#define RESULTSSTATE_H

#include <memory>
#include <string>
#include <vector>
#include <Observer.h>
#include "SceneState.h"
#include "glm/glm.hpp"

namespace dae
{
	class Scene;
	class GameObject;
}

namespace highScoreHandling
{
	struct PlayerScore;
}

class InitialsComponent;
class ResultsState final : public SceneState, public dae::Observer<InitialsComponent>
{
public:
	explicit ResultsState() = default;
	virtual ~ResultsState();

	ResultsState(const ResultsState&) = delete;
	ResultsState(ResultsState&&) noexcept = delete;
	ResultsState& operator= (const ResultsState&) = delete;
	ResultsState& operator= (ResultsState&&) noexcept = delete;

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void OnSuspend() override;
	virtual void OnResume() override;

	virtual void Notify(InitialsComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<InitialsComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid(dae::Subject<InitialsComponent>* pSubject) override;

private:
	void LoadPlayerScore(dae::Scene& scene, const std::string& name);
	void LoadTopFive(dae::Scene& scene) const;
	void LoadInputInfo(dae::Scene& scene) const;
	void CreateCycleCommands(dae::GameObject* pObject) const;
	void CreateLeaveButton(dae::Scene& scene) const;


	bool m_ReadyToLeave{false};
	int m_RefreshCount{};
	const glm::u8vec4 m_Yellow{ 254, 241, 0, 255 };
	static const std::string m_SceneName;
	static const std::string m_TemporaryInitials;


	std::vector<dae::Subject<InitialsComponent>*> m_pVecObservedSubjects;
};


#endif // !RESULTSSTATE_H