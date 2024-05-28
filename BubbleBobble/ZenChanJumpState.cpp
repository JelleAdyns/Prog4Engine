#include "ZenChanJumpState.h"
#include "SpriteComponent.h"
#include "ZenChanComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>

ZenChanJumpState::ZenChanJumpState(dae::GameObject* pZenChan, ZenChanComponent* pZenChanComp, bool isAngry) :
	ZenChanState{},
	m_IsAngry{ isAngry },
	m_TimeToJump{ isAngry ? 0.5f : 1.f },
	m_pZenChan{ pZenChan },
	m_pZenChanComp{ pZenChanComp },
	m_pPhysicsComp{ pZenChan->GetComponent<dae::PhysicsComponent>() },
	m_pSpriteComp{ pZenChan->GetComponent<SpriteComponent>() },
	m_pCollisionComp{ pZenChan->GetComponent<dae::CollisionComponent>() }
{};


std::unique_ptr<ZenChanState> ZenChanJumpState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<ZenChanCaughtState>(m_pZenChan, pCollidedObject);
		}
	}

	auto deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
	m_TimeBeforeJump += deltaTime;
	m_TimeBeforeFlip += deltaTime;

	if (m_CheckIfLanded)
	{
		if (m_pPhysicsComp->GetVelocity().y > 0.f) return std::make_unique<ZenChanFallingState>(m_pZenChan, m_pZenChanComp, m_IsAngry);
	}
	else
	{
		if (m_TimeBeforeFlip >= m_TimeToJump / 2.f)
		{
			m_pSpriteComp->Flip();
			m_TimeBeforeFlip = 0.f;
		}

		if (m_TimeBeforeJump >= m_TimeToJump)
		{
			m_pPhysicsComp->SetVelocityY(m_JumpVelocity);
			m_CheckIfLanded = true;
		}
	}

	return nullptr;
}
void ZenChanJumpState::OnEnter()
{

	m_pSpriteComp->SetCol(0);
	m_pSpriteComp->SetUpdate(false);

	m_pSpriteComp->Flip();

	m_pPhysicsComp->SetVelocityX(0);
}
void ZenChanJumpState::OnExit()
{

	m_pSpriteComp->SetUpdate(true);

}