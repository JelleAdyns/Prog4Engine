#ifndef MAITAFALLINGSTATE_H
#define MAITAFALLINGSTATE_H

#include "MaitaState.h"
#include "PlayerComponent.h"

namespace dae
{
	class GameObject;
	class PhysicsComponent;
	class CollisionComponent;
}

class EnemyComponent;
class SpriteComponent;
class FloorCheckingComponent;
class MaitaFallingState final : public MaitaState
{
public:
	explicit MaitaFallingState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry);
	virtual ~MaitaFallingState() = default;

	MaitaFallingState(const MaitaFallingState&) = delete;
	MaitaFallingState(MaitaFallingState&&) noexcept = delete;
	MaitaFallingState& operator= (const MaitaFallingState&) = delete;
	MaitaFallingState& operator= (MaitaFallingState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void NotifyPlayerObservers(PlayerComponent* pSubject) override;
private:

	bool m_IsAngry;
	float m_PlayerXPos{};

	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
};


#endif // !MAITAFALLINGSTATE_H