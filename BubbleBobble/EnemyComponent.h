#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H

#include <Component.h>
#include <Subject.h>

namespace dae
{
	class CollisionComponent;
}
class PlayerComponent;
class EnemyComponent final : public dae::Component
{
public:
	explicit EnemyComponent(dae::GameObject* pOwner, PlayerComponent* pObserver);
	virtual ~EnemyComponent() = default;
	EnemyComponent(const EnemyComponent&) = delete;
	EnemyComponent(EnemyComponent&&) noexcept = delete;
	EnemyComponent& operator= (const EnemyComponent&) = delete;
	EnemyComponent& operator= (EnemyComponent&&) noexcept = delete;

	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

private:

	dae::CollisionComponent* m_pCollisionComp;
	std::unique_ptr<dae::Subject<EnemyComponent>> m_pCollided;
};



#endif // !ENEMYCOMPONENT_H
