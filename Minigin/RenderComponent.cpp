#include "RenderComponent.h"
#include "Textcomponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "GameObject.h"

namespace dae
{
	RenderComponent::RenderComponent(GameObject* pOwner):
		Component{pOwner}, m_pTransform{nullptr}
	{}

	void RenderComponent::Update() { if (!m_pTransform) GetOwner()->GetTransform(); }

	void RenderComponent::Render() const
	{
		float posX = m_pTransform->GetPosition().x;
		float posY = m_pTransform->GetPosition().y;

		for (auto& pair : m_pMapTexturesToRender)
		{
			Renderer::GetInstance().RenderTexture(*(pair.second), posX, posY);
		}
	}
}
