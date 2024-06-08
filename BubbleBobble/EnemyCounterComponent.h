#ifndef ENEMYCOUNTERCOMPONENT_H
#define ENEMYCOUNTERCOMPONENT_H

#include <Component.h>
#include <Observer.h>
#include <Command.h>
#include "EnemyComponent.h"

namespace dae
{
	class PhysicsComponent;
}

class SpriteComponent;
class EnemyCounterComponent final : public dae::Component, public dae::Observer<EnemyComponent>
{
public:


	explicit EnemyCounterComponent(dae::GameObject* pOwner, std::unique_ptr<dae::Command>& pNextLevelCommand);
	virtual ~EnemyCounterComponent();
	EnemyCounterComponent(const EnemyCounterComponent&) = delete;
	EnemyCounterComponent(EnemyCounterComponent&&) noexcept = delete;
	EnemyCounterComponent& operator= (const EnemyCounterComponent&) = delete;
	EnemyCounterComponent& operator= (EnemyCounterComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(EnemyComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<EnemyComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid(dae::Subject<EnemyComponent>* pSubject) override;

private:

	int m_AmountOfEnemies{};

	float m_Timer{};
	float m_TimeForLevelSwitch{ 5.f };


	std::unique_ptr<dae::Command> m_pNextLevelCommand;
	std::vector<dae::Subject<EnemyComponent>*> m_pVecObservedSubjects;
};



#endif // !ENEMYCOUNTERCOMPONENT_H
