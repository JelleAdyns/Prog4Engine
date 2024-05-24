#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <memory>
#include "Component.h"
#include "Subject.h"

namespace dae
{
    class PhysicsComponent;
    class CollisionComponent final : public Component
    {
    public:

        enum class CollidingSide
        {
            Top = 0b0001,
            Left = 0b0010,
            Bottom = 0b0100,
            Right = 0b1000
        };

        explicit CollisionComponent(GameObject* pOwner, const std::string& collisionTag);
        explicit CollisionComponent(GameObject* pOwner, const glm::vec2& posOffset, const glm::vec2& size, const std::string& collisionTag);
        virtual ~CollisionComponent();

        CollisionComponent(const CollisionComponent&) = delete;
        CollisionComponent(CollisionComponent&&) noexcept = delete;
        CollisionComponent& operator= (const CollisionComponent&) = delete;
        CollisionComponent& operator= (CollisionComponent&&) noexcept = delete;

        virtual void Start() override;
        virtual void Update() override;
        virtual void PrepareImGuiRender() override;

        uint8_t GetCollisionFlags() const;

        const glm::vec2& GetOverlappedDistance() const;
        const glm::vec2& GetOffset() const;
        const glm::vec2& GetSize() const;

        void SetCollision(bool collisionOn);
        void CheckForCollision(const std::string& collisionTag);
        void CheckForCollision(const glm::vec2& alternativeOffset, const glm::vec2& alternativeSize, const std::string& collisionTag);

    private:
        //const glm::vec2& GetOffset();

        struct Box
        {
            float top;
            float left;
            float bottom;
            float right;
        };

        bool IsColliding(const Box& box, const Box& otherBox, CollidingSide sideToTest);

        const std::string m_CollisionTag{};

        bool m_HasPhysicsComponent;
        bool m_CollisionOn;
        uint8_t m_CollisionFlags;

        glm::vec2 m_PosOffset;
        glm::vec2 m_Size;

        glm::vec2 m_OverlappedDistance;

        static std::vector<CollisionComponent*> m_pVecAllCollisionComponents;
    };

}

#endif // !COLLISIONCOMPONENT_H