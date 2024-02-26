#include <string>
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "ImageComponent.h"
namespace dae
{
	ImageComponent::ImageComponent(const std::shared_ptr<GameObject>& pOwner, const std::string& texturePath ):
		Component{pOwner}, m_pTexture{ ResourceManager::GetInstance().LoadTexture(texturePath)}
	{
		using ThisType = std::remove_reference<decltype(*this)>::type;
		m_pOwner.lock()->GetComponent<RenderComponent>()->AddTexture<ThisType>(m_pTexture);
	}
	
	void ImageComponent::Update(){}
}