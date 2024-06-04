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

class InventoryComponent;
class Achievements;
class ScoreUIComponent final : public dae::Component, public dae::Observer<InventoryComponent>
{
public:
	explicit ScoreUIComponent(dae::GameObject* pOwner, int startScore, Achievements* pObserver);
	virtual ~ScoreUIComponent();

	ScoreUIComponent(const ScoreUIComponent&) = delete;
	ScoreUIComponent(ScoreUIComponent&&) noexcept = delete;
	ScoreUIComponent& operator= (const ScoreUIComponent&) = delete;
	ScoreUIComponent& operator= (ScoreUIComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(InventoryComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<InventoryComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

	int GetScore() const;

private:
	int m_TotalScore;

	dae::TextComponent* m_pTextComponent;
	std::unique_ptr<dae::Subject<ScoreUIComponent>> m_pScoreChanged;

    std::vector<dae::Subject<InventoryComponent>*> m_pVecObservedSubjects;
};


#endif // !SCOREUICOMPONENT_H