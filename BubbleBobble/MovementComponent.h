#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include <Component.h>
#include <Command.h>

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
	void RegisterJumpOnBubbleCommand() const;
	void UnRegisterJumpOnBubbleCommand() const;
	void RegisterMoveCommands() const;
	void UnRegisterMoveCommands() const;
	void RegisterAttackCommand() const;
	void UnRegisterAttackCommand() const;

	uint8_t GetPlayerIndex() const { return m_PlayerIndex; }
private:
	uint8_t m_PlayerIndex{};

	float m_JumpVelocity;
	float m_MoveSpeed;

	std::shared_ptr<dae::Command> m_pJumpCommand;
	std::shared_ptr<dae::Command> m_pJumpOnBubbleCommand;
	std::shared_ptr<dae::Command> m_MoveRightCommand;
	std::shared_ptr<dae::Command> m_MoveLeftCommand;
	std::shared_ptr<dae::Command> m_StopMovingCommand;
	std::shared_ptr<dae::Command> m_AttackCommand;

	static uint8_t m_NrOfPlayers;
};
#endif // !MOVEMENTCOMPONENT_H