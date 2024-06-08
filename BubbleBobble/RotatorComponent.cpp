#include "RotatorComponent.h"
#include "GameObject.h"
#include <numbers>

RotatorComponent::RotatorComponent(dae::GameObject* pOwner):
	dae::Component{pOwner},
	m_Radius{},
	m_Angle{},
	m_Speed{}
{

}

RotatorComponent::RotatorComponent(dae::GameObject* pOwner, float radius, float startDegrees, float speed):
	dae::Component{ pOwner },
	m_Radius{ radius },
	m_Angle{startDegrees* static_cast<float>(std::numbers::pi) / 180},
	m_Speed{ speed }
{

}
void RotatorComponent::Start()
{
}
void RotatorComponent::Update()
{
	constexpr float Pi2 = 2 * static_cast<float>(std::numbers::pi);

	m_Angle += m_Speed * dae::GameTime::GetInstance().GetDeltaTime();
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