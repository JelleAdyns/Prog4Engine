#include "PhysicsComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "GameTime.h"

namespace dae
{
	float PhysicsComponent::m_Gravity{ 9.81f };

	PhysicsComponent::PhysicsComponent(GameObject* pOwner) :
		Component{ pOwner },
		m_UseGravity{true},
		m_Velocity{}
	{
	}

	PhysicsComponent::~PhysicsComponent()
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

		if(m_UseGravity) m_Velocity.y += m_Gravity * GameTime::GetInstance().GetFixedTimeStep();
		
		GetOwner()->SetLocalPos(
            currPos.x + m_Velocity.x * GameTime::GetInstance().GetFixedTimeStep(), 
            currPos.y + m_Velocity.y * GameTime::GetInstance().GetFixedTimeStep()
        );

		//m_Velocity.x = 0;
	}

	void PhysicsComponent::AddVelocity(const glm::vec2& Velocity)
	{
		m_Velocity.x += Velocity.x;
		m_Velocity.y += Velocity.y;
	}

	void PhysicsComponent::SetVelocityX(float xVelocity)
	{
        m_Velocity.x = xVelocity;
	}

    void PhysicsComponent::SetVelocityY(float yVelocity)
    {
        m_Velocity.y = yVelocity;
    }

	const glm::vec2& PhysicsComponent::GetVelocity() const
	{
		return m_Velocity;
	}
	void PhysicsComponent::StopGravity()
	{
		m_UseGravity = false;
	}
	void PhysicsComponent::StartGravity()
	{
		m_UseGravity = true;
	}
	void PhysicsComponent::SetGravity(float gravity)
	{
		m_Gravity = gravity;
	}
}
