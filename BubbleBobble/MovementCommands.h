#ifndef MOVEMENTCOMMANDS_H
#define MOVEMENTCOMMANDS_H

#include <GameObjectCommand.h>
#include <PhysicsComponent.h>
#include <GameTime.h>
#include <AudioLocator.h>
#include "WallCheckingComponent.h"
#include "FloorCheckingComponent.h"
#include "Game.h"
//--------------------------
// Moving
class MoveCommand final : public dae::GameObjectCommand
{
public:
	MoveCommand(const std::unique_ptr<dae::GameObject>& pObject, float xSpeed):
		MoveCommand{ pObject.get(), xSpeed }
	{

	}
	MoveCommand(dae::GameObject* pObject, float xSpeed) :
		dae::GameObjectCommand{ pObject},
		m_XDirectionSpeed{xSpeed}
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
			m_pPhysicsComponent->AddVelocity(glm::vec2{ m_XDirectionSpeed, 0 });
		}

	}

private:
	float m_XDirectionSpeed;
	dae::PhysicsComponent* m_pPhysicsComponent;
	WallCheckingComponent* m_pWallCheckingComponent;
};

//--------------------------
// Stop Moving
class StopMovingCommand final : public dae::GameObjectCommand
{
public:
	StopMovingCommand(const std::unique_ptr<dae::GameObject>& pObject) :
		StopMovingCommand{ pObject.get() }
	{}
	StopMovingCommand(dae::GameObject* pObject) :
		dae::GameObjectCommand{ pObject }
	{
		m_pPhysicsComponent = GetGameObject()->GetComponent<dae::PhysicsComponent>();
	}
	virtual ~StopMovingCommand() = default;

	StopMovingCommand(const StopMovingCommand&) = delete;
	StopMovingCommand(StopMovingCommand&&) noexcept = delete;
	StopMovingCommand& operator= (const StopMovingCommand&) = delete;
	StopMovingCommand& operator= (StopMovingCommand&&) noexcept = delete;
	virtual void Execute() const override
	{
		m_pPhysicsComponent->SetVelocityX(0);
	}

private:
	dae::PhysicsComponent* m_pPhysicsComponent;
};

//--------------------------
// Jump
class JumpCommand final : public dae::GameObjectCommand
{
public:
	JumpCommand(const std::unique_ptr<dae::GameObject>& pObject, float jumpVelocity) :
		JumpCommand{ pObject.get(), jumpVelocity }
	{}
	JumpCommand(dae::GameObject* pObject, float jumpVelocity) :
		dae::GameObjectCommand{ pObject },
		m_JumpVelocity{ jumpVelocity }
	{
		m_pPhysicsComponent = GetGameObject()->GetComponent<dae::PhysicsComponent>();
		m_pFloorCheckingComponent = GetGameObject()->GetComponent<FloorCheckingComponent>();
	};
	virtual ~JumpCommand() = default;

	JumpCommand(const JumpCommand&) = delete;
	JumpCommand(JumpCommand&&) noexcept = delete;
	JumpCommand& operator= (const JumpCommand&) = delete;
	JumpCommand& operator= (JumpCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		if (m_pFloorCheckingComponent->IsOnGround())
		{
			m_pPhysicsComponent->SetVelocityY(m_JumpVelocity);

			//this code is in here purely for demonstration purposes
			dae::AudioLocator::GetAudioService().AddSound("Sounds/Jump.wav", static_cast<dae::SoundID>(Game::SoundEvent::Jump));
			dae::AudioLocator::GetAudioService().PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::Jump), 120, false);
		}
	}
private:
	const float m_JumpVelocity;
	dae::PhysicsComponent* m_pPhysicsComponent;
	FloorCheckingComponent* m_pFloorCheckingComponent;
};

#endif // !MOVEMENTCOMMANDS_H