#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"

namespace dae
{

	void GameObject::Update()
	{
		for (auto& component: m_pMapComponents)
		{
			component.second->Update();
		}
	}
	void GameObject::Render() const
	{
		if (const auto& pComp = GetComponent<dae::RenderComponent>(); pComp != nullptr) pComp->Render();
	}


}

