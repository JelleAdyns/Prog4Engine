#ifndef MOVEMENTCOMMANDS_H
#define MOVEMENTCOMMANDS_H

#include <GameObjectCommand.h>
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameTime.h>
#include "CollisionTags.h"
#include <AudioLocator.h>
#include "WallCheckingComponent.h"
#include "FloorCheckingComponent.h"
#include "BubbleComponent.h"
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
			auto currentVelocity = m_pPhysicsComponent->GetVelocity().x;
			if ((m_XDirectionSpeed > 0 && currentVelocity <= 0) ||
				(m_XDirectionSpeed < 0 && currentVelocity >= 0))
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
	JumpCommand(const std::unique_ptr<dae::GameObject>& pObject, float jumpVelocity, bool checkForBubble = false) :
		JumpCommand{ pObject.get(), jumpVelocity, checkForBubble }
	{}
	JumpCommand(dae::GameObject* pObject, float jumpVelocity, bool checkForBubble = false) :
		dae::GameObjectCommand{ pObject },
		m_CheckForBubble{checkForBubble},
		m_JumpVelocity{ jumpVelocity }
	{
		m_pPhysicsComponent = GetGameObject()->GetComponent<dae::PhysicsComponent>();
		m_pCollisionComponent = GetGameObject()->GetComponent<dae::CollisionComponent>();
		m_pFloorCheckingComponent = GetGameObject()->GetComponent<FloorCheckingComponent>();
	};
	virtual ~JumpCommand() = default;

	JumpCommand(const JumpCommand&) = delete;
	JumpCommand(JumpCommand&&) noexcept = delete;
	JumpCommand& operator= (const JumpCommand&) = delete;
	JumpCommand& operator= (JumpCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		if (m_CheckForBubble)
		{
			dae::GameObject* bubble = m_pCollisionComponent->CheckForCollision(collisionTags::bubbleTag);
			if (bubble)
			{
				auto flags = m_pCollisionComponent->GetCollisionFlags();
				const auto bottomFlag = static_cast<char>(dae::CollisionComponent::CollidingSide::Bottom);

				if ((flags & bottomFlag) == bottomFlag)
				{
					if (bubble->GetComponent<BubbleComponent>()->IsFloating())
						m_pPhysicsComponent->SetVelocityY(m_JumpVelocity);
				}
			}
			else
			{
				dae::GameObject* occupiedBubble = m_pCollisionComponent->CheckForCollision(collisionTags::OccupiedBubbleTag);

				if (occupiedBubble)
				{
					auto flags = m_pCollisionComponent->GetCollisionFlags();
					const auto bottomFlag = static_cast<char>(dae::CollisionComponent::CollidingSide::Bottom);

					if ((flags & bottomFlag) == bottomFlag)
					{
						if (occupiedBubble->GetComponent<BubbleComponent>()->IsFloating())
							m_pPhysicsComponent->SetVelocityY(m_JumpVelocity);
					}
				}
			}
			
		}
		else
		{
			if (m_pFloorCheckingComponent->IsOnGround())
			{
				m_pPhysicsComponent->SetVelocityY(m_JumpVelocity);

				//this code is in here purely for demonstration purposes
				dae::AudioLocator::GetAudioService().AddSound("Sounds/Jump.wav", static_cast<dae::SoundID>(Game::SoundEvent::Jump));
				dae::AudioLocator::GetAudioService().PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::Jump), 120, false);
			}
		}
		
	}
private:
	const bool m_CheckForBubble;
	const float m_JumpVelocity;
	dae::PhysicsComponent* m_pPhysicsComponent;
	dae::CollisionComponent* m_pCollisionComponent;
	FloorCheckingComponent* m_pFloorCheckingComponent;
};

#endif // !MOVEMENTCOMMANDS_H