#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H

#include <Component.h>
#include <Subject.h>
#include "States.h"

namespace dae
{
	class PhysicsComponent;
}
class PlayerComponent;
class EnemyComponent final : public dae::Component
{
public:
	enum class EnemyType
	{
		ZenChan,
		Maita
	};

	explicit EnemyComponent(dae::GameObject* pOwner, EnemyType enemyType);
	virtual ~EnemyComponent() = default;
	EnemyComponent(const EnemyComponent&) = delete;
	EnemyComponent(EnemyComponent&&) noexcept = delete;
	EnemyComponent& operator= (const EnemyComponent&) = delete;
	EnemyComponent& operator= (EnemyComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void AddPlayerObserver(PlayerComponent* pSubject);

	const std::vector<dae::Subject<PlayerComponent>*>& GetPlayerSubjects() const { return m_SubjectsForState; }

private:
	void UpdateStates();

	const EnemyType m_EnemyType;
	std::unique_ptr<EnemyState> m_pCurrState;

	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;

	std::vector<dae::Subject<PlayerComponent>*> m_SubjectsForState;
};



#endif // !ENEMYCOMPONENT_H
