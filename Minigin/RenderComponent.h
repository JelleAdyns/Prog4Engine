#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

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
			requires std::derived_from<T, Component>
		void AddTexture(const std::unique_ptr<Texture2D>& pTexture, bool flipTexture = false)
		{
			m_pMapTexturesToRender[typeid(T)] = std::make_pair(pTexture.get(), flipTexture);
		}
		template <typename T>
			requires std::derived_from<T,Component>
		void SetFlipped(bool flipTexture)
		{
			m_pMapTexturesToRender.at(typeid(T)).second = flipTexture;
		}

	private:

		bool m_UseMiddle;
		glm::vec2 m_Pos;

		std::map<std::type_index, std::pair<Texture2D*, bool>> m_pMapTexturesToRender;
	};

}

#endif // !RENDERCOMPONENT_H