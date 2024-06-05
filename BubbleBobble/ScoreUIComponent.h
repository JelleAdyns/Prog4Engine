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
class PlayerComponent;
class Achievements;
class ScoreUIComponent final : public dae::Component, public dae::Observer<InventoryComponent>, public dae::Observer<PlayerComponent>
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
	virtual void SetSubjectPointersInvalid(dae::Subject<InventoryComponent>* pSubject) override;

	virtual void Notify (PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid(dae::Subject<PlayerComponent>* pSubject) override;

	int GetScore() const;

private:
	int m_TotalScore;

	dae::TextComponent* m_pTextComponent;
	std::unique_ptr<dae::Subject<ScoreUIComponent>> m_pScoreChanged;

    dae::Subject<InventoryComponent>* m_pVecObservedInventorySubject;
    dae::Subject<PlayerComponent>* m_pVecObservedPlayerSubject;
};


#endif // !SCOREUICOMPONENT_H