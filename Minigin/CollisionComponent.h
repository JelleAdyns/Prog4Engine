#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <memory>
#include "Component.h"
namespace dae
{
    class PhysicsComponent;
    class CollisionComponent final : public Component
    {
    public:
        explicit CollisionComponent(GameObject* pOwner, const glm::vec2& posOffset, const glm::vec2& size);
        virtual ~CollisionComponent() = default;

        CollisionComponent(const CollisionComponent&) = delete;
        CollisionComponent(CollisionComponent&&) noexcept = delete;
        CollisionComponent& operator= (const CollisionComponent&) = delete;
        CollisionComponent& operator= (CollisionComponent&&) noexcept = delete;

        virtual void Update() override;
        virtual void PrepareImGuiRender() override;

        void CheckForCollision() const;
    private:

        glm::vec2 m_PosOffset;
        glm::vec2 m_Size;


    };

}

#endif // !COLLISIONCOMPONENT_H