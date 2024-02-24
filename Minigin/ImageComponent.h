#pragma once
#include <memory>
#include "Component.h"
namespace dae
{
	class Texture2D;
	class ImageComponent final : public Component
	{
	public:
		explicit ImageComponent(const std::shared_ptr<GameObject>& pOwner, const std::string& texturePath);
		virtual ~ImageComponent() = default;

		ImageComponent(const ImageComponent&) = delete;
		ImageComponent(ImageComponent&&) noexcept = delete;
		ImageComponent& operator= (const ImageComponent&) = delete;
		ImageComponent& operator= (ImageComponent&&) noexcept = delete;
		//virtual void FixedUpdate(float) override;
		virtual void Update() override;
		
	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};

}

