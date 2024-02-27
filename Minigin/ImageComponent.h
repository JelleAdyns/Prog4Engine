#pragma once
#include <memory>
#include "Component.h"
namespace dae
{
	class Texture2D;
	class RenderComponent;
	class ImageComponent final : public Component
	{
	public:
		explicit ImageComponent(GameObject* pOwner, const std::string& texturePath);
		virtual ~ImageComponent() = default;

		ImageComponent(const ImageComponent&) = delete;
		ImageComponent(ImageComponent&&) noexcept = delete;
		ImageComponent& operator= (const ImageComponent&) = delete;
		ImageComponent& operator= (ImageComponent&&) noexcept = delete;

		virtual void Update() override;
		
	private:
		std::shared_ptr<Texture2D> m_pTexture;

		RenderComponent* m_pRenderComponent;
	};

}

