#include "TransformComponent.h"
#include "GameObject.h"

namespace dae
{
	TransformComponent::TransformComponent(GameObject* pOwner) :
		Component{ pOwner }, m_position{ }
	{
	}
	TransformComponent::TransformComponent(GameObject* pOwner, const glm::vec2& pos):
		Component{pOwner}, m_position{pos }
	{}

	void TransformComponent::Start()
	{
	}

	void TransformComponent::Update(){}
	void TransformComponent::PrepareImGuiRender(){}

	void dae::TransformComponent::SetPosition(const glm::vec2& newPos)
	{
		m_position = newPos;
	}

}
