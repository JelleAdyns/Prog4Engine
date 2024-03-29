#pragma once
#include <map>
#include <typeindex>
#include <cassert>
#include "Component.h"
#include "Texture2D.h"
namespace dae
{
	class TransformComponent;
	class RenderComponent final : public Component
	{
	public:
		explicit RenderComponent(GameObject* pOwner, bool useMiddleOfTextures = false);
		virtual ~RenderComponent() = default;

		RenderComponent(const RenderComponent&) = delete;
		RenderComponent(RenderComponent&&) noexcept = delete;
		RenderComponent& operator= (const RenderComponent&) = delete;
		RenderComponent& operator= (RenderComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		void Render() const;

		template <typename T>
		void AddTexture(const std::unique_ptr<Texture2D>& pTexture)
		{
			assert((std::derived_from<T,Component>));
			m_pMapTexturesToRender[typeid(T)] = pTexture.get();
		}

	private:

		glm::vec2 m_Pos;
		bool m_UseMiddle;
		//TransformComponent* m_pTransformComponent;
		std::map<std::type_index,	Texture2D*> m_pMapTexturesToRender;
	};

}

