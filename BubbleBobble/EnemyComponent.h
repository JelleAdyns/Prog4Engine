#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H

#include <Component.h>
#include <Subject.h>
#include "States.h"

namespace dae
{
	class CollisionComponent;
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

	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void AddPlayerObserver(PlayerComponent* pObserver);

	float GetSpeed() const { return m_Speed; }
	const std::vector<dae::Subject<PlayerComponent>*>& GetPlayerSubjects() const { return m_SubjectsForRunState; }

private:
	void UpdateStates();

	float m_Speed{ 60.f };
	const EnemyType m_EnemyType;
	std::unique_ptr<EnemyState> m_pCurrState;

	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	SpriteComponent* m_pSpriteComp;
	std::unique_ptr<dae::Subject<EnemyComponent>> m_pCollided;

	std::vector<dae::Subject<PlayerComponent>*> m_SubjectsForRunState;
};



#endif // !ENEMYCOMPONENT_H
