#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <GameObjectCommand.h>
#include <PhysicsComponent.h>
#include <GameTime.h>
#include "WallCheckingComponent.h"

class MoveCommand final : public dae::GameObjectCommand
{
public:
	MoveCommand(const std::unique_ptr<dae::GameObject>& pObject, int xSpeed) :
		dae::GameObjectCommand{ pObject.get() },
		m_XDirectionSpeed{static_cast<float>(xSpeed)}
	{
		m_pPhysicsComponent = GetGameObject()->GetComponent<dae::PhysicsComponent>();
		m_pWallCheckingComponent = GetGameObject()->GetComponent<WallCheckingComponent>();
	}
	virtual ~MoveCommand() = default;

	MoveCommand(const MoveCommand&) = delete;
	MoveCommand(MoveCommand&&) noexcept = delete;
	MoveCommand& operator= (const MoveCommand&) = delete;
	MoveCommand& operator= (MoveCommand&&) noexcept = delete;
	virtual void Execute() const override
	{
		if ((!m_pWallCheckingComponent->CollidingWithLeft() && m_XDirectionSpeed < 0) || 
			(!m_pWallCheckingComponent->CollidingWithRight() && m_XDirectionSpeed > 0))
		{
			auto localPos = GetGameObject()->GetLocalPosition();
			m_pPhysicsComponent->AddVelocity(glm::vec2{ m_XDirectionSpeed,0 });
		}
	}

private:
	float m_XDirectionSpeed;
	dae::PhysicsComponent* m_pPhysicsComponent;
	WallCheckingComponent* m_pWallCheckingComponent;
};

#endif // !MOVECOMMAND_H