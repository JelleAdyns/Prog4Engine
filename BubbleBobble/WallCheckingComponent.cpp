#include "WallCheckingComponent.h"
#include "CollisionComponent.h"
#include "CollisionTags.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Minigin.h"
#include <imgui.h>

WallCheckingComponent::WallCheckingComponent(dae::GameObject* pOwner, const glm::vec2& offset, const glm::vec2& size):
	dae::Component{pOwner},
	m_Offset{offset},
	m_Size{size},
	m_pCollisionComponent{},
	m_pPhysicsComponent{}
{
}

void WallCheckingComponent::Start()
{
	if (!m_pCollisionComponent) m_pCollisionComponent = GetOwner()->GetComponent<dae::CollisionComponent>();
	if (!m_pPhysicsComponent) m_pPhysicsComponent = GetOwner()->GetComponent<dae::PhysicsComponent>();
}

void WallCheckingComponent::Update()
{
	
	m_pCollisionComponent->CheckForCollision(m_Offset, m_Size, collisionTags::wallTag);

	auto flags = m_pCollisionComponent->GetCollisionFlags();

	const auto leftFlag = static_cast<char>(dae::CollisionComponent::CollidingSide::Left);
	const auto rightFlag = static_cast<char>(dae::CollisionComponent::CollidingSide::Right);

	auto overlappedDistance = m_pCollisionComponent->GetOverlappedDistance();

	m_CollidingLeft = false;
	m_CollidingRight = false;

	
	if((flags & leftFlag) == leftFlag)
	{
		auto localPos = GetOwner()->GetLocalPosition();

		GetOwner()->SetLocalPos(localPos.x + overlappedDistance.x, localPos.y);
		m_CollidingLeft = true;
	}
	if ((flags & rightFlag) == rightFlag)
	{
		auto localPos = GetOwner()->GetLocalPosition();

		GetOwner()->SetLocalPos(localPos.x - overlappedDistance.x, localPos.y);
		m_CollidingRight = true;
	}

}

void WallCheckingComponent::PrepareImGuiRender()
{
#ifndef NDEBUG
	auto scale = dae::Minigin::GetWindowScale();

	ImGui::Begin("Collision");
	// ImGui::SetWindowSize(ImVec2{ float( Minigin::GetWindowSize().x* scale), float( Minigin::GetWindowSize().y*scale )});
	// ImGui::SetWindowPos(ImVec2{});
	float top = GetOwner()->GetWorldPosition().y + m_Offset.y;
	float left = GetOwner()->GetWorldPosition().x + m_Offset.x;
	ImGui::GetWindowDrawList()->AddRect(
		ImVec2(left * scale, top * scale),
		ImVec2((left + m_Size.x) * scale, (top + m_Size.y) * scale),
		IM_COL32(0, 255, 0, 255));
	ImGui::End();
#endif //!NDEBUG
}

bool WallCheckingComponent::CollidingWithLeft() const
{
	return m_CollidingLeft;
}

bool WallCheckingComponent::CollidingWithRight() const
{
	return m_CollidingRight;
}
