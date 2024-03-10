#include "RotatorComponent.h"
#include "GameObject.h"


namespace dae
{
	RotatorComponent::RotatorComponent(GameObject* pOwner):
		Component{pOwner},
		m_Radius{},
		m_Angle{},
		m_Speed{}
	{

	}

	RotatorComponent::RotatorComponent(GameObject* pOwner, float radius, float speed):
		Component{ pOwner },
		m_Radius{ radius },
		m_Angle{},
		m_Speed{ speed }
	{

	}
	void RotatorComponent::Update()
	{
		m_Angle += m_Speed * GameTime::GetInstance().GetDeltaTime();

		GetOwner()->SetLocalPos(
			glm::vec2{cos(m_Angle) * m_Radius,
					  sin(m_Angle) * m_Radius }
		);
		
	}
	void RotatorComponent::PrepareImGuiRender() {}

	void RotatorComponent::SetRadius(float radius)
	{
		m_Radius = radius;
	}
	void RotatorComponent::SetSpeed(float speed)
	{
		m_Speed = speed;
	}
}
