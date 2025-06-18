#include "CollisionComponent.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Minigin.h"
#include <algorithm>
#include <imgui.h>

namespace dae
{
    std::vector<CollisionComponent*> CollisionComponent::m_pVecAllCollisionComponents{};

    CollisionComponent::CollisionComponent(GameObject* pOwner, const std::string& collisionTag):
        CollisionComponent(pOwner, glm::vec2{}, glm::vec2{}, collisionTag)
    {
    }

    CollisionComponent::CollisionComponent(GameObject* pOwner, const glm::vec2& posOffset, const glm::vec2& size, const std::string& collisionTag) :
        Component{ pOwner },
        m_CollisionTag{collisionTag},
        m_HasPhysicsComponent{ nullptr},
        m_CollisionOn{true},
        m_CollisionFlags{},
        m_PosOffset{ posOffset },
        m_Size{ size }
    {
        m_pVecAllCollisionComponents.push_back(this);
    }

    CollisionComponent::~CollisionComponent()
    {
        m_pVecAllCollisionComponents.erase(
            std::remove(m_pVecAllCollisionComponents.begin(), m_pVecAllCollisionComponents.end(), this),
            m_pVecAllCollisionComponents.cend());
    }

    void CollisionComponent::Start()
    {
        m_HasPhysicsComponent = GetOwner()->HasComponent<PhysicsComponent>();
    }

    void CollisionComponent::Update()
    {
        
        //if (m_HasPhysicsComponent) CheckForCollision(CollisionType::Other);

    }

    void CollisionComponent::PrepareImGuiRender()
    {
#ifndef NDEBUG
        auto scale = Minigin::GetWindowScale();

        ImGui::Begin("Collision");
       // ImGui::SetWindowSize(ImVec2{ float( Minigin::GetWindowSize().x* scale), float( Minigin::GetWindowSize().y*scale )});
       // ImGui::SetWindowPos(ImVec2{});
        float top = GetOwner()->GetWorldPosition().y + m_PosOffset.y;
        float left = GetOwner()->GetWorldPosition().x + m_PosOffset.x;
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(left * scale, top * scale),
            ImVec2((left + m_Size.x) * scale, (top + m_Size.y) * scale),
            IM_COL32(255, 0, 0, 255));
        ImGui::End();
#endif //!NDEBUG
    }

    void CollisionComponent::SetTag(const std::string& newTag)
    {
        m_CollisionTag = newTag;
    }

    void CollisionComponent::SetCollision(bool collisionOn)
    {
        m_CollisionOn = collisionOn;
    }

    GameObject* CollisionComponent::CheckForCollision(const std::string& collisionTag)
    {
        return CheckForCollision(m_PosOffset, m_Size, collisionTag);
    }
    GameObject* CollisionComponent::CheckForCollision(const glm::vec2& alternativeOffset, const glm::vec2& alternativeSize, const std::string& collisionTag)
    {
        
        GameObject* pCollidedObject{ nullptr };

        m_CollisionFlags = 0;
        m_OverlappedDistance = {};

        if(m_CollisionOn)
        {
            const auto& worldPos = GetOwner()->GetWorldPosition();

            Box box{};
            box.top = worldPos.y + alternativeOffset.y;
            box.left = worldPos.x + alternativeOffset.x;
            box.bottom = box.top + alternativeSize.y;
            box.right = box.left + alternativeSize.x;
            
            std::for_each(m_pVecAllCollisionComponents.cbegin(), m_pVecAllCollisionComponents.cend(),
                [&](CollisionComponent* pCollComponent)
                {

                    if (pCollComponent->m_CollisionOn &&                    // check if collision is needed
                        m_CollisionFlags == 0 &&                            // check if not already collided with something
                        pCollComponent != this &&                           // don't check for itself
                        collisionTag == pCollComponent->m_CollisionTag)     // check for correct collision tag
                    {
                        const auto& otherWorldPos = pCollComponent->GetOwner()->GetWorldPosition();
                        const auto& otherOffset = pCollComponent->m_PosOffset;
                        const auto& otherSize = pCollComponent->m_Size;

                        Box otherBox{};
                        otherBox.top = otherWorldPos.y + otherOffset.y;
                        otherBox.left = otherWorldPos.x + otherOffset.x;
                        otherBox.bottom = otherBox.top + otherSize.y;
                        otherBox.right = otherBox.left + otherSize.x;

                        if (IsColliding(box, otherBox, CollidingSide::Top))
                        {
                            m_CollisionFlags |= static_cast<char>(CollidingSide::Top);
                            m_OverlappedDistance.y = otherBox.bottom - box.top;
                            pCollidedObject = pCollComponent->GetOwner();
                        }
                        if (IsColliding(box, otherBox, CollidingSide::Left))
                        {
                            m_CollisionFlags |= static_cast<char>(CollidingSide::Left);
                            m_OverlappedDistance.x = otherBox.right - box.left;
                            pCollidedObject = pCollComponent->GetOwner();
                        }
                        if (IsColliding(box, otherBox, CollidingSide::Bottom))
                        {
                            m_CollisionFlags |= static_cast<char>(CollidingSide::Bottom);
                            m_OverlappedDistance.y = box.bottom - otherBox.top;
                            pCollidedObject = pCollComponent->GetOwner();
                        }
                        if (IsColliding(box, otherBox, CollidingSide::Right))
                        {
                            m_CollisionFlags |= static_cast<char>(CollidingSide::Right);
                            m_OverlappedDistance.x = box.right - otherBox.left;
                            pCollidedObject = pCollComponent->GetOwner();
                        }
                    }

                });
        }

        return pCollidedObject;
    }

    uint8_t CollisionComponent::GetCollisionFlags() const
    {
        return m_CollisionFlags;
    }

    const glm::vec2& CollisionComponent::GetOverlappedDistance() const
    {
        return m_OverlappedDistance;
    }

    const glm::vec2& CollisionComponent::GetOffset() const
    {
        return m_PosOffset;
    }

    const glm::vec2& CollisionComponent::GetSize() const
    {
        return m_Size;
    }

    bool CollisionComponent::IsColliding(const Box& box, const Box& otherBox, CollidingSide sideToTest)
    {
        switch (sideToTest)
        {
        case CollidingSide::Top:

            return (box.top <= otherBox.bottom && box.top >= otherBox.top &&
                (
                    (box.right <= otherBox.right && box.right >= otherBox.left) ||
                    (box.left >= otherBox.left && box.left <= otherBox.right) ||
                    (box.right >= otherBox.right && box.left <= otherBox.left) ||
                    (box.right <= otherBox.right && box.left >= otherBox.left)
                    )
                );

            break;
        case CollidingSide::Left:

            return (box.left <= otherBox.right && box.left >= otherBox.left &&
                (
                    (box.top >= otherBox.top && box.top <= otherBox.bottom) ||
                    (box.bottom <= otherBox.bottom && box.bottom >= otherBox.top) ||
                    (box.top <= otherBox.top && box.bottom >= otherBox.bottom) ||
                    (box.top >= otherBox.top && box.bottom <= otherBox.bottom)
                    )
                );
            
            break;
        case CollidingSide::Bottom:

            return (box.bottom >= otherBox.top && box.bottom <= otherBox.bottom &&
                (
                    (box.right <= otherBox.right && box.right >= otherBox.left) ||
                    (box.left >= otherBox.left && box.left <= otherBox.right) ||
                    (box.right >= otherBox.right && box.left <= otherBox.left) ||
                    (box.right <= otherBox.right && box.left >= otherBox.left)
                    )
                );

            break;
        case CollidingSide::Right:
            
            return (box.right >= otherBox.left && box.right <= otherBox.right &&
                (
                    (box.top >= otherBox.top && box.top <= otherBox.bottom) ||
                    (box.bottom <= otherBox.bottom && box.bottom >= otherBox.top) ||
                    (box.top <= otherBox.top && box.bottom >= otherBox.bottom) ||
                    (box.top >= otherBox.top && box.bottom <= otherBox.bottom)
                    )
                );

            break;
        default:
            return false;
            break;
        }
    }


}
