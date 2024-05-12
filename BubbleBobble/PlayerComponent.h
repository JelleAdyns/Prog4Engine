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
}

class SpriteComponent;
class PlayerComponent final : public dae::Component, public dae::Observer<SpriteComponent>
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

	void Shoot();

	float GetJumpVelocity() const { return m_JumpVelocity; }
	float GetMoveVelocity() const { return m_MoveVelocity; }
	uint8_t GetPlayerIndex() const { return m_PlayerIndex; }
private:

	uint8_t m_PlayerIndex{};

	bool m_IsShooting{};
	bool m_IsInvincible{};

	int m_Health{5};
	float m_JumpVelocity{ -160.f };
	float m_MoveVelocity{ 60.f };


	std::unique_ptr<PlayerState> m_pCurrState{};
	
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSubjects;
	static uint8_t m_NrOfPlayers;

	void UpdateStates();
};

#endif // !PLAYERCOMPONENT_H

