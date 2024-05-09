#ifndef IMAGECOMPONENT_H
#define IMAGECOMPONENT_H

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
		explicit ImageComponent(GameObject* pOwner, std::unique_ptr<Texture2D>&& pTexture);
		virtual ~ImageComponent() = default;

		ImageComponent(const ImageComponent&) = delete;
		ImageComponent(ImageComponent&&) noexcept = delete;
		ImageComponent& operator= (const ImageComponent&) = delete;
		ImageComponent& operator= (ImageComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

	private:
		std::unique_ptr<Texture2D> m_pTexture;

		RenderComponent* m_pRenderComponent;
	};

}

#endif // !IMAGECOMPONENT_H