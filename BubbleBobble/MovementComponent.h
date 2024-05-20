#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include <Component.h>

class MovementComponent final : public dae::Component
{
public:
	explicit MovementComponent(dae::GameObject* pOwner, float jumpVelocity, float moveSpeed);
	virtual ~MovementComponent();

	MovementComponent(const MovementComponent&) = delete;
	MovementComponent(MovementComponent&&) noexcept = delete;
	MovementComponent& operator= (const MovementComponent&) = delete;
	MovementComponent& operator= (MovementComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void RegisterJumpCommand() const;
	void UnRegisterJumpCommand() const;
	void RegisterMoveCommands() const;
	void UnRegisterMoveCommands() const;
	void RegisterAttackCommand() const;
	void UnRegisterAttackCommand() const;

	uint8_t GetPlayerIndex() const { return m_PlayerIndex; }
private:
	uint8_t m_PlayerIndex{};

	float m_JumpVelocity;
	float m_MoveSpeed;

	static uint8_t m_NrOfPlayers;
};
#endif // !MOVEMENTCOMPONENT_H