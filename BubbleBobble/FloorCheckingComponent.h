#ifndef FLOORCHECKINGCOMPONENT_H
#define FLOORCHECKINGCOMPONENT_H

#include "Component.h"
#include <string>


namespace dae
{
	class GameObject;
	class CollisionComponent;
	class PhysicsComponent;
}


class FloorCheckingComponent final : public dae::Component
{
public:
	explicit FloorCheckingComponent(dae::GameObject* pOwner, const glm::vec2& offset, const glm::vec2& size);
	virtual ~FloorCheckingComponent() = default;
	FloorCheckingComponent(const FloorCheckingComponent&) = delete;
	FloorCheckingComponent(FloorCheckingComponent&&) noexcept = delete;
	FloorCheckingComponent& operator= (const FloorCheckingComponent&) = delete;
	FloorCheckingComponent& operator= (FloorCheckingComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	bool IsOnGround() const;
private:

	void HandleCollision();

	bool m_IsOnGround;
	glm::vec2 m_Offset;
	glm::vec2 m_Size;
	dae::CollisionComponent* m_pCollisionComponent;
	dae::PhysicsComponent* m_pPhysicsComponent;
};

#endif // !FLOORCHECKINGCOMPONENT_H