#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <memory>
#include "Component.h"
namespace dae
{
    class CollisionComponent;
	class PhysicsComponent final : public Component
	{
	public:
		explicit PhysicsComponent(GameObject* pOwner);
		virtual ~PhysicsComponent() = default;

		PhysicsComponent(const PhysicsComponent&) = delete;
		PhysicsComponent(PhysicsComponent&&) noexcept = delete;
		PhysicsComponent& operator= (const PhysicsComponent&) = delete;
		PhysicsComponent& operator= (PhysicsComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		void FixedUpdate();

        void SetVelocityX(float xVelocity);
        void SetVelocityY(float yVelocity);
	private:

		float m_Mass;

		glm::vec2 m_Velocity;

        CollisionComponent* m_pPhysicsComponent;

		static const float m_Gravity;
	};

}

#endif // !PHYSICSCOMPONENT_H