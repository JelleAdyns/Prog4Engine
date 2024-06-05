#ifndef MAITARUNSTATE_H
#define MAITARUNSTATE_H

#include "MaitaState.h"
#include "PlayerComponent.h"
#include "SpriteComponent.h"

namespace dae
{
	class GameObject;
	class CollisionComponent;
	class PhysicsComponent;
}

class EnemyComponent;
class WallCheckingComponent;
class FloorCheckingComponent;
class MaitaRunState final : public MaitaState
{
public:
	explicit MaitaRunState(dae::GameObject* pMaita, EnemyComponent* pEnemyComp, bool isAngry = false);
	virtual ~MaitaRunState() = default;

	MaitaRunState(const MaitaRunState&) = delete;
	MaitaRunState(MaitaRunState&&) noexcept = delete;
	MaitaRunState& operator= (const MaitaRunState&) = delete;
	MaitaRunState& operator= (MaitaRunState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void NotifyPlayerObservers(PlayerComponent* pSubject) override;
	virtual void Attack() override;
private:

	void OnEnterPlayable();
	void OnEnterRegular();
	std::unique_ptr<EnemyState> UpdatePlayable();
	std::unique_ptr<EnemyState> UpdateRegular();

	static constexpr SpriteComponent::SpriteInfo m_RunInfo{ .rowUpdate{false}, .rowNumber{0}, .nrOfCols{5}, .frameTime{0.1f} };
	static constexpr float m_GeneralSpeed{ 50.f };
	const float m_Speed;

	float m_AttackDelayTimer{};
	const float m_AttackDelay{ 0.7f };

	bool m_HasToJump{ false };
	bool m_HasToAttack{ false };
	bool m_IsAngry;

	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
	WallCheckingComponent* m_pWallCheckingComp;
	FloorCheckingComponent* m_pFloorCheckingComp;
	dae::CollisionComponent* m_pCollisionComp;
};


#endif // !MAITARUNSTATE_H