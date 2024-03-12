#include "RotatorComponent.h"
#include "GameObject.h"
#include <numbers>


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
		constexpr float Pi2 = 2 * static_cast<float>(std::numbers::pi);

		m_Angle += m_Speed * GameTime::GetInstance().GetDeltaTime();
		if (m_Angle >= Pi2) m_Angle -= Pi2;

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
