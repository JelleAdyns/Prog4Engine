#ifndef MAITACOMPONENT_H
#define MAITACOMPONENT_H

#include <Component.h>
#include <Subject.h>
#include "States.h"

namespace dae
{
	class PhysicsComponent;
}
class PlayerComponent;
class MaitaComponent final : public dae::Component
{
public:

	explicit MaitaComponent(dae::GameObject* pOwner);
	virtual ~MaitaComponent() = default;
	MaitaComponent(const MaitaComponent&) = delete;
	MaitaComponent(MaitaComponent&&) noexcept = delete;
	MaitaComponent& operator= (const MaitaComponent&) = delete;
	MaitaComponent& operator= (MaitaComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void AddPlayerObserver(PlayerComponent* pSubject);

	const std::vector<dae::Subject<PlayerComponent>*>& GetPlayerSubjects() const { return m_SubjectsForState; }

private:
	void UpdateStates();

	std::unique_ptr<MaitaState> m_pCurrState;

	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;

	std::vector<dae::Subject<PlayerComponent>*> m_SubjectsForState;
};



#endif // !MAITACOMPONENT_H
