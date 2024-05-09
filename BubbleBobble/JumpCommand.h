#ifndef JUMPCOMMAND_H
#define JUMPCOMMAND_H

#include <GameObjectCommand.h>
#include <PhysicsComponent.h>
#include <AudioLocator.h>
#include "FloorCheckingComponent.h"
#include "Game.h"

class JumpCommand final : public dae::GameObjectCommand
{
public:
    JumpCommand(const std::unique_ptr<dae::GameObject>& pObject, float jumpVelocity) :
        dae::GameObjectCommand{pObject.get()},
        m_JumpVelocity{jumpVelocity}
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
        if(m_pFloorCheckingComponent->IsOnGround())
        {
            m_pPhysicsComponent->SetVelocityY(m_JumpVelocity);

            //this code is in here purely for demonstration purposes
            dae::AudioLocator::GetAudioService().AddSound("Jump.wav", static_cast<dae::SoundID>(Game::SoundEvent::Jump));
            dae::AudioLocator::GetAudioService().PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::Jump), 120, false);
        }
    }
private:
    const float m_JumpVelocity;
    dae::PhysicsComponent* m_pPhysicsComponent;
    FloorCheckingComponent* m_pFloorCheckingComponent;
};

#endif // !JUMPCOMMAND_H