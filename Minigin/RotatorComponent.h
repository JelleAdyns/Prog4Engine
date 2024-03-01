#pragma once
#include <memory>
#include "Component.h"
namespace dae
{
	class Texture2D;
	class TransformComponent;
	class RotatorComponent final : public Component
	{
	public:
		explicit RotatorComponent(GameObject* pOwner);
		explicit RotatorComponent(GameObject* pOwner, const glm::vec2& center, float radius);
		virtual ~RotatorComponent() = default;

		RotatorComponent(const RotatorComponent&) = delete;
		RotatorComponent(RotatorComponent&&) noexcept = delete;
		RotatorComponent& operator= (const RotatorComponent&) = delete;
		RotatorComponent& operator= (RotatorComponent&&) noexcept = delete;

		virtual void Update() override;

		void SetCenter(const glm::vec2& center);
		void SetRadius(float radius);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		glm::vec2 m_Center;
		float m_Radius;
		float m_Angle;
	};

}

