#include "RenderComponent.h"
#include "Textcomponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "GameObject.h"

namespace dae
{
	RenderComponent::RenderComponent(GameObject* pOwner):
		Component{pOwner}
	{}

	void RenderComponent::Update(){}

	void RenderComponent::Render() const
	{
		const auto& pTransform = GetOwner()->GetComponent<dae::TransformComponent>();
		float posX = pTransform->GetPosition().x;
		float posY = pTransform->GetPosition().y;

		for (auto& pair : m_pMapTexturesToRender)
		{
			Renderer::GetInstance().RenderTexture(*(pair.second), posX, posY);
		}
	}
}
