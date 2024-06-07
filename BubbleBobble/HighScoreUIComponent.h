#ifndef HIGHSCOREUICOMPONENT_H
#define HIGHSCOREUICOMPONENT_H

#include "Observer.h"
#include "Subject.h"
#include "Component.h"
#include <string>
#include <vector>

namespace dae
{
	class TextComponent;
	class GameObject;
}

class ScoreUIComponent;
class HighScoreUIComponent final : public dae::Component, public dae::Observer<ScoreUIComponent>
{
public:
	explicit HighScoreUIComponent(dae::GameObject* pOwner);
	virtual ~HighScoreUIComponent();

	HighScoreUIComponent(const HighScoreUIComponent&) = delete;
	HighScoreUIComponent(HighScoreUIComponent&&) noexcept = delete;
	HighScoreUIComponent& operator= (const HighScoreUIComponent&) = delete;
	HighScoreUIComponent& operator= (HighScoreUIComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(ScoreUIComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<ScoreUIComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid(dae::Subject<ScoreUIComponent>* pSubject) override;

private:
	int m_CurrentHighScore;

	dae::TextComponent* m_pTextComponent;

	std::vector<dae::Subject<ScoreUIComponent>*> m_pVecObservedSubjects;
};


#endif // !HIGHSCOREUICOMPONENT_H