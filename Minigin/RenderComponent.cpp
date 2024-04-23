#include "RenderComponent.h"
#include "Textcomponent.h"
#include "Renderer.h"
#include "GameObject.h"

namespace dae
{
	RenderComponent::RenderComponent(GameObject* pOwner, bool useMiddleOfTextures):
		Component{ pOwner }, m_UseMiddle{useMiddleOfTextures}, m_Pos{}
	{}

	void RenderComponent::Update()
	{
		m_Pos = GetOwner()->GetWorldPosition();

		for (auto& pair : m_pMapTexturesToRender)
		{
			pair.second->SetDstRect(m_Pos);
		}
	}
	void RenderComponent::PrepareImGuiRender() { }

	void RenderComponent::Render() const
	{
		const auto& renderer = Renderer::GetInstance();
		
		for (auto& pair : m_pMapTexturesToRender)
		{
			if (m_UseMiddle)
			{
				glm::ivec2 pos
				{
					static_cast<int>(m_Pos.x - pair.second->GetDstRect().w / 2.f),
					static_cast<int>(m_Pos.y - pair.second->GetDstRect().h / 2.f)
				};
				pair.second->SetDstRect(pos);
			}

			renderer.RenderTexture(*(pair.second), pair.second->GetSrcRect(), pair.second->GetDstRect());
		}
		
	}
}
