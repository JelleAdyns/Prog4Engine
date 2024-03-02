#include "RenderComponent.h"
#include "Textcomponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "GameObject.h"

namespace dae
{
	RenderComponent::RenderComponent(GameObject* pOwner, bool useMiddleOfTextures):
		Component{ pOwner }, m_Pos{}, m_UseMiddle{useMiddleOfTextures}
	{}

	void RenderComponent::Update() 
	{
		m_Pos = GetOwner()->GetWorldPosition();
	}

	void RenderComponent::Render() const
	{
		const auto& renderer = Renderer::GetInstance();
		if (m_UseMiddle)
		{
			for (auto& pair : m_pMapTexturesToRender)
			{
				float posX{ m_Pos.x - pair.second->GetSize().x / 2 };
				float posY{ m_Pos.y - pair.second->GetSize().y / 2 };
				renderer.RenderTexture(*(pair.second), posX, posY);
			}
		}
		else
		{
			for (auto& pair : m_pMapTexturesToRender)
			{
				renderer.RenderTexture(*(pair.second), m_Pos.x, m_Pos.y);
			}
		}
		
	}
}
