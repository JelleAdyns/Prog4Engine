#include "RotatorComponent.h"
#include "GameObject.h"


namespace dae
{
	RotatorComponent::RotatorComponent(GameObject* pOwner):
		Component{pOwner},
		m_Center{},
		m_Radius{},
		m_Angle{},
		m_Speed{},
		m_Clockwise {}
	{

	}

	RotatorComponent::RotatorComponent(GameObject* pOwner, const glm::vec2& center, float radius, float speed, bool clockwise):
		Component{ pOwner },
		m_Center{ center },
		m_Radius{ radius },
		m_Angle{},
		m_Speed{ speed },
		m_Clockwise{ clockwise }
	{

	}
	void RotatorComponent::Update()
	{
		m_Angle += m_Speed * GameTime::GetInstance().GetDeltaTime();

		GetOwner()->SetLocalPos(
			glm::vec2{ m_Center.x + cos(m_Angle) * m_Radius,
						m_Center.y + (m_Clockwise ? sin(m_Angle) : -sin(m_Angle)) * m_Radius }
		);
		
	}

	void RotatorComponent::SetCenter(const glm::vec2& center)
	{
		m_Center = center;
	}
	void RotatorComponent::SetRadius(float radius)
	{
		m_Radius = radius;
	}
}
