#include "RenderComponent.h"
#include "Textcomponent.h"
#include "Renderer.h"
#include "GameObject.h"

namespace dae
{
	RenderComponent::RenderComponent(GameObject* pOwner, bool useMiddleOfTextures):
		Component{ pOwner }, m_UseMiddle{useMiddleOfTextures}, m_Pos{}
	{}

	void RenderComponent::Start()
	{
	}

	void RenderComponent::Update()
	{
		m_Pos = GetOwner()->GetWorldPosition();

		for (auto& pair : m_pMapTexturesToRender)
		{
			pair.second.first->SetDstRect(m_Pos);
		}
	}
	void RenderComponent::PrepareImGuiRender() { }

	void RenderComponent::Render() const
	{
		const auto& renderer = Renderer::GetInstance();
		
		for (auto& pair : m_pMapTexturesToRender)
		{
			auto& textureAndBoolPair = pair.second;

			if (m_UseMiddle)
			{
				glm::ivec2 pos
				{
					static_cast<int>(m_Pos.x - textureAndBoolPair.first->GetDstRect().w / 2.f),
					static_cast<int>(m_Pos.y - textureAndBoolPair.first->GetDstRect().h / 2.f)
				};
				textureAndBoolPair.first->SetDstRect(pos);
			}

			renderer.RenderTexture(*(textureAndBoolPair.first), textureAndBoolPair.first->GetSrcRect(), textureAndBoolPair.first->GetDstRect(), textureAndBoolPair.second);
				
		}
		
	}
}
