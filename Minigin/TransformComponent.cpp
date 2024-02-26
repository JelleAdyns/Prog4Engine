#include "TransformComponent.h"

namespace dae
{
	TransformComponent::TransformComponent(const std::shared_ptr<GameObject>& pOwner):
		Component{pOwner}, m_position{}
	{}

	void TransformComponent::Update(){}
	void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

}
