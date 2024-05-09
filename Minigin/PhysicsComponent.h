#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <memory>
#include <vector>
#include "Component.h"
#include "Subject.h"

namespace dae
{
    class CollisionComponent;
	class PhysicsComponent final : public Component
	{
	public:
		explicit PhysicsComponent(GameObject* pOwner);
		virtual ~PhysicsComponent();

		PhysicsComponent(const PhysicsComponent&) = delete;
		PhysicsComponent(PhysicsComponent&&) noexcept = delete;
		PhysicsComponent& operator= (const PhysicsComponent&) = delete;
		PhysicsComponent& operator= (PhysicsComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		void FixedUpdate();

        void AddVelocity(const glm::vec2& Velocity);
        void SetVelocityX(float xVelocity);
        void SetVelocityY(float yVelocity);

		const glm::vec2& GetVelocity() const;

		static void SetGravity(float gravity);
	private:

		float m_Mass;

		glm::vec2 m_Velocity;

		static float m_Gravity;
	};

}

#endif // !PHYSICSCOMPONENT_H