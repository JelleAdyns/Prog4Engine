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

    CollisionComponent::CollisionComponent(GameObject* pOwner, CollisionType collisionType):
        CollisionComponent(pOwner, glm::vec2{}, glm::vec2{}, collisionType)
    {
    }

    CollisionComponent::CollisionComponent(GameObject* pOwner, const glm::vec2& posOffset, const glm::vec2& size, CollisionType collisionType) :
        Component{ pOwner },
        m_CollisionType{collisionType},
        m_HasPhysicsComponent{ nullptr},
        m_CollisionOn{true},
        m_CollisionFlags{},
        m_PosOffset{ posOffset },
        m_Size{ size },
        m_GeneralOffset{posOffset},
        m_GeneralSize{size}
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
        
        m_PosOffset = m_GeneralOffset;
        m_Size = m_GeneralSize;
        /*if (m_HasPhysicsComponent) */CheckForCollision(CollisionType::Other);

    }

    void CollisionComponent::PrepareImGuiRender()
    {
        auto scale = Minigin::GetWindowScale();

        ImGui::Begin("Collision");
       // ImGui::SetWindowSize(ImVec2{ float( Minigin::GetWindowSize().x* scale), float( Minigin::GetWindowSize().y*scale )});
       // ImGui::SetWindowPos(ImVec2{});
        float top = GetOwner()->GetWorldPosition().y + m_GeneralOffset.y;
        float left = GetOwner()->GetWorldPosition().x + m_GeneralOffset.x;
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(left * scale, top * scale),
            ImVec2((left + m_GeneralSize.x) * scale, (top + m_GeneralSize.y) * scale),
            IM_COL32(255, 0, 0, 255));
        ImGui::End();
    }


    void CollisionComponent::SetOffset(const glm::vec2& newOffset)
    {
        m_PosOffset = newOffset;
    }

    void CollisionComponent::SetSize(const glm::vec2& newSize)
    {
        m_Size = newSize;
    }

    void CollisionComponent::SetCollision(bool collisionOn)
    {
        m_CollisionOn = collisionOn;
    }

    /*const glm::vec2 CollisionComponent::GetCollisionPos()
    {
        const auto& pos = GetOwner()->GetWorldPosition();
        return glm::vec2{ pos.x + m_PosOffset.x, pos.y + m_PosOffset.y };
    }*/

    //const glm::vec2& CollisionComponent::GetOffset()
    //{
    //    return m_PosOffset;
    //}

    /*const glm::vec2& CollisionComponent::GetCollisionSize()
    {
        return m_Size;
    }*/

    void CollisionComponent::CheckForCollision(CollisionType typeToCheck)
    {
        const auto& worldPos = GetOwner()->GetWorldPosition();

        Box box{};
        box.top = worldPos.y + m_PosOffset.y;
        box.left = worldPos.x + m_PosOffset.x;
        box.bottom = box.top + m_Size.y;
        box.right = box.left + m_Size.x;

        m_CollisionFlags = 0;
        m_OverlappedDistance = {};

        std::for_each(m_pVecAllCollisionComponents.cbegin(), m_pVecAllCollisionComponents.cend(), [&](CollisionComponent* pCollComponent)
            {
                if( pCollComponent != this &&
                    typeToCheck == pCollComponent->m_CollisionType &&
                    m_CollisionOn)
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
                        m_OverlappedDistance.y = otherBox.bottom - box.top ;
                    }
                    if (IsColliding(box, otherBox, CollidingSide::Left))
                    {
                        m_CollisionFlags |= static_cast<char>(CollidingSide::Left);
                        m_OverlappedDistance.x = otherBox.right - box.left;
                    }
                    if (IsColliding(box, otherBox, CollidingSide::Bottom))
                    {
                        m_CollisionFlags |= static_cast<char>(CollidingSide::Bottom);
                        m_OverlappedDistance.y = box.bottom - otherBox.top;
                    }
                    if (IsColliding(box, otherBox, CollidingSide::Right))
                    {
                        m_CollisionFlags |= static_cast<char>(CollidingSide::Right);
                        m_OverlappedDistance.x = box.right - otherBox.left;
                    }
                }

            });
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
