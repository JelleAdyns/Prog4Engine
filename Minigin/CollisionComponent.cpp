#include "CollisionComponent.h"
#include "GameObject.h"

namespace dae
{
    CollisionComponent::CollisionComponent(GameObject* pOwner, const glm::vec2& posOffset, const glm::vec2& size):
        Component{pOwner},
        m_PosOffset{posOffset},
        m_Size{size}
    {
    }

    void CollisionComponent::Update()
    {
    }

    void CollisionComponent::PrepareImGuiRender()
    {
    }

    void CollisionComponent::CheckForCollision() const
    {
        
    }

}
