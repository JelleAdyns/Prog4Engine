#include "PhysicsComponent.h"
#include "GameObject.h"
#include "GameTime.h"

namespace dae
{
	float PhysicsComponent::m_Gravity{ 9.81f };

	PhysicsComponent::PhysicsComponent(GameObject* pOwner) :
		Component{ pOwner },
		m_Mass{},
		m_Velocity{}
	{
	}

	void PhysicsComponent::Update()
	{
	}

	void PhysicsComponent::PrepareImGuiRender()
	{
	}

	void PhysicsComponent::FixedUpdate()
	{
		auto currPos = GetOwner()->GetLocalPosition();
		if (currPos.y > 224) currPos.y = 0;
		m_Velocity.y += m_Gravity * GameTime::GetInstance().GetFixedTimeStep();

		if (m_Velocity.y > 200) m_Velocity.y = 200;
		GetOwner()->SetLocalPos(
            currPos.x + m_Velocity.x * GameTime::GetInstance().GetFixedTimeStep(), 
            currPos.y + m_Velocity.y * GameTime::GetInstance().GetFixedTimeStep()
        );
	}

	void PhysicsComponent::SetVelocityX(float xVelocity)
	{
        m_Velocity.x = xVelocity;
	}

    void PhysicsComponent::SetVelocityY(float yVelocity)
    {
        m_Velocity.y = yVelocity;
    }

	void PhysicsComponent::SetGravity(float gravity)
	{
		m_Gravity = gravity;
	}

}
