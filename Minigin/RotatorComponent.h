#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include <memory>
#include "Component.h"
namespace dae
{
	class RotatorComponent final : public Component
	{
	public:
		explicit RotatorComponent(GameObject* pOwner);
		explicit RotatorComponent(GameObject* pOwner, float radius, float speed = 1.f);
		virtual ~RotatorComponent() = default;

		RotatorComponent(const RotatorComponent&) = delete;
		RotatorComponent(RotatorComponent&&) noexcept = delete;
		RotatorComponent& operator= (const RotatorComponent&) = delete;
		RotatorComponent& operator= (RotatorComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		void SetRadius(float radius);
		void SetSpeed(float speed);

	private:
		
		float m_Radius;
		float m_Angle;
		float m_Speed;
	};

}

#endif // !ROTATORCOMPONENT_H