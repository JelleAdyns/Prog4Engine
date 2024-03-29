#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* pOwner);
		virtual ~TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		const glm::vec2& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec2& newPos);
	private:

		glm::vec2 m_position;
	};

}
	
