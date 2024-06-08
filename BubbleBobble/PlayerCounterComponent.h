#ifndef PLAYERCOUNTERCOMPONENT_H
#define PLAYERCOUNTERCOMPONENT_H

#include <Component.h>
#include <Observer.h>
#include <Command.h>
#include "PlayerComponent.h"

namespace dae
{
	class PhysicsComponent;
}

class SpriteComponent;
class TimerComponent;
class PlayerCounterComponent final : public dae::Component, public dae::Observer<PlayerComponent>
{
public:


	explicit PlayerCounterComponent(dae::GameObject* pOwner);
	virtual ~PlayerCounterComponent();
	PlayerCounterComponent(const PlayerCounterComponent&) = delete;
	PlayerCounterComponent(PlayerCounterComponent&&) noexcept = delete;
	PlayerCounterComponent& operator= (const PlayerCounterComponent&) = delete;
	PlayerCounterComponent& operator= (PlayerCounterComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid(dae::Subject<PlayerComponent>* pSubject) override;

private:

	int m_AmountOfPlayers{};

	TimerComponent* m_pTimerComp;

	std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSubjects;
};



#endif // !PLAYERCOUNTERCOMPONENT_H
