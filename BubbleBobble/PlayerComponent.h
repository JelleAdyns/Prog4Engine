#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include <Component.h>
#include <Observer.h>
#include <Subject.h>
#include "PlayerState.h"

namespace dae
{
	class GameObject;
	class PhysicsComponent;
	class CollisionComponent;
}

class SpriteComponent;
class EnemyComponent;
class PlayerComponent final : public dae::Component, public dae::Observer<SpriteComponent>, public dae::Observer<EnemyComponent>
{
public:
	explicit PlayerComponent(dae::GameObject* pOwner);
	virtual ~PlayerComponent();

	PlayerComponent(const PlayerComponent&) = delete;
	PlayerComponent(PlayerComponent&&) noexcept = delete;
	PlayerComponent& operator= (const PlayerComponent&) = delete;
	PlayerComponent& operator= (PlayerComponent&&) noexcept = delete;

	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(SpriteComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject) override;
	virtual void Notify(EnemyComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<EnemyComponent>* pSubject) override;

	void Shoot();
	bool IsHit() const;
	void Respawn();

	dae::Subject<PlayerComponent>* GetSubject() const;
	glm::vec2 GetPos() const;
	float GetJumpVelocity() const { return m_JumpVelocity; }
	float GetMoveVelocity() const { return m_MoveVelocity; }
	uint8_t GetPlayerIndex() const { return m_PlayerIndex; }
private:

	uint8_t m_PlayerIndex{};

	bool m_IsShooting{};
	bool m_IsInvincible{};
	bool m_IsHit{};

	int m_SpriteRowcount{0};
	int m_Health{5};
	float m_JumpVelocity{ -160.f };
	float m_MoveVelocity{ 60.f };
	float m_InvincibilityTimer{};
	float m_InvincibilityMaxTime{1.f};

	std::unique_ptr<PlayerState> m_pCurrState{};
	
	dae::PhysicsComponent* m_pPhysicsComp;
	dae::CollisionComponent* m_pCollisionComp;
	SpriteComponent* m_pSpriteComp;

	std::unique_ptr<dae::Subject<PlayerComponent>> m_pPosChecked;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSpriteSubjects;
	std::vector<dae::Subject<EnemyComponent>*> m_pVecObservedEnemySubjects;

	static uint8_t m_NrOfPlayers;

	void UpdateStates();
};

#endif // !PLAYERCOMPONENT_H
