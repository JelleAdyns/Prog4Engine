#include "RenderComponent.h"
#include "Textcomponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "GameObject.h"

namespace dae
{
	RenderComponent::RenderComponent(GameObject* pOwner):
		Component{ pOwner }, m_Pos{}
	{}

	void RenderComponent::Update() 
	{
		m_Pos = GetOwner()->GetWorldPosition();
	}

	void RenderComponent::Render() const
	{
		for (auto& pair : m_pMapTexturesToRender)
		{
			Renderer::GetInstance().RenderTexture(*(pair.second), m_Pos.x, m_Pos.y);
		}
	}
}
