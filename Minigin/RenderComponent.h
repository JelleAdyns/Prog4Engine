#pragma once
#include <map>
#include <typeindex>
#include <cassert>

#include "Component.h"
#include "Texture2D.h"
namespace dae
{
	
	class RenderComponent final : public Component
	{
	public:
		explicit RenderComponent(const std::shared_ptr<GameObject>& pOwner);
		virtual ~RenderComponent() = default;

		RenderComponent(const RenderComponent&) = delete;
		RenderComponent(RenderComponent&&) noexcept = delete;
		RenderComponent& operator= (const RenderComponent&) = delete;
		RenderComponent& operator= (RenderComponent&&) noexcept = delete;
		//virtual void FixedUpdate(float) override;
		virtual void Update() override;
		void Render() const;

		template <typename T>
		void AddTexture(const std::shared_ptr<Texture2D>& pTexture)
		{
			assert((std::derived_from<T,Component>));
			m_pMapTexturesToRender[typeid(T)] = pTexture;
		}

	private:
		std::map<std::type_index, std::shared_ptr<Texture2D>> m_pMapTexturesToRender;
	};

}

