#ifndef SCOREUICOMPONENT_H
#define SCOREUICOMPONENT_H

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

class PickUpComponent;
class Achievements;
class ScoreUIComponent final : public dae::Component, public dae::Observer<PickUpComponent>
{
public:
	explicit ScoreUIComponent(dae::GameObject* pOwner, Achievements* pObserver);
	virtual ~ScoreUIComponent();

	ScoreUIComponent(const ScoreUIComponent&) = delete;
	ScoreUIComponent(ScoreUIComponent&&) noexcept = delete;
	ScoreUIComponent& operator= (const ScoreUIComponent&) = delete;
	ScoreUIComponent& operator= (ScoreUIComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(PickUpComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PickUpComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

	int GetScore() const;

private:
	int m_Score;
	dae::TextComponent* m_pTextComponent;
	std::unique_ptr<dae::Subject<ScoreUIComponent>> m_pScoreChanged;

    std::vector<dae::Subject<PickUpComponent>*> m_pVecObservedSubjects;
};


#endif // !SCOREUICOMPONENT_H