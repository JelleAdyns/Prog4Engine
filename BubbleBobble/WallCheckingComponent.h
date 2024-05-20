#ifndef WALLCHECKINGCOMPONENT_H
#define WALLCHECKINGCOMPONENT_H

#include "Component.h"
#include <string>


namespace dae
{
	class GameObject;
	class CollisionComponent;
	class PhysicsComponent;
}


class WallCheckingComponent final : public dae::Component
{
public:
	explicit WallCheckingComponent(dae::GameObject* pOwner, const glm::vec2& offset, const glm::vec2& size);
	virtual ~WallCheckingComponent() = default;
	WallCheckingComponent(const WallCheckingComponent& other) = delete;
	WallCheckingComponent(WallCheckingComponent&& other) = delete;
	WallCheckingComponent& operator=(const WallCheckingComponent& other) = delete;
	WallCheckingComponent& operator=(WallCheckingComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	bool CollidingWithLeft() const;
	bool CollidingWithRight() const;
private:

	glm::vec2 m_Offset;
	glm::vec2 m_Size;
	dae::CollisionComponent* m_pCollisionComponent;
	dae::PhysicsComponent* m_pPhysicsComponent;

	bool m_CollidingLeft{};
	bool m_CollidingRight{};

	static const int m_WallThickness{ 16 };
};

#endif // !WALLCHECKINGCOMPONENT_H
