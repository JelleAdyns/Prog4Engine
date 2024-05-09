#include "FloorCheckingComponent.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Minigin.h"
#include <imgui.h>

FloorCheckingComponent::FloorCheckingComponent(dae::GameObject* pOwner, const glm::vec2& offset, const glm::vec2& size):
	dae::Component{ pOwner },
	m_Offset{ offset },
	m_Size{ size },
	m_pCollisionComponent{},
	m_pPhysicsComponent{}
{
}

void FloorCheckingComponent::Update()
{
	if (!m_pCollisionComponent) m_pCollisionComponent = GetOwner()->GetComponent<dae::CollisionComponent>();
	if (!m_pPhysicsComponent) m_pPhysicsComponent = GetOwner()->GetComponent<dae::PhysicsComponent>();

	m_pCollisionComponent->SetOffset(m_Offset);
	m_pCollisionComponent->SetSize(m_Size);

	m_pCollisionComponent->CheckForCollision(dae::CollisionComponent::CollisionType::Platform);

	auto flags = m_pCollisionComponent->GetCollisionFlags();

	//const auto leftFlag = static_cast<char>(dae::CollisionComponent::CollidingSide::Left);
	const auto bottomFlag = static_cast<char>(dae::CollisionComponent::CollidingSide::Bottom);

	auto overlappedDistance = m_pCollisionComponent->GetOverlappedDistance();

	m_IsOnGround = false;
	if ((flags & bottomFlag) == bottomFlag)
	{
		if (m_pPhysicsComponent->GetVelocity().y >= 0)
		{
			auto localPos = GetOwner()->GetLocalPosition();
			GetOwner()->SetLocalPos(localPos.x, localPos.y - overlappedDistance.y);
			m_pPhysicsComponent->SetVelocityY(0);
			m_IsOnGround = true;
		}
	}
}

void FloorCheckingComponent::PrepareImGuiRender()
{
	auto scale = dae::Minigin::GetWindowScale();

	ImGui::Begin("Collision");
	// ImGui::SetWindowSize(ImVec2{ float( Minigin::GetWindowSize().x* scale), float( Minigin::GetWindowSize().y*scale )});
	// ImGui::SetWindowPos(ImVec2{});
	float top = GetOwner()->GetWorldPosition().y + m_Offset.y;
	float left = GetOwner()->GetWorldPosition().x + m_Offset.x;
	ImGui::GetWindowDrawList()->AddRect(
		ImVec2(left * scale, top * scale),
		ImVec2((left + m_Size.x) * scale, (top + m_Size.y) * scale),
		IM_COL32(0, 0, 255, 255));
	ImGui::End();
}

bool FloorCheckingComponent::IsOnGround() const
{
	return m_IsOnGround;
}
