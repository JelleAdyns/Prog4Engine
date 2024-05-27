#ifndef MAITAJUMPSTATE_H
#define MAITAJUMPSTATE_H

#include "MaitaState.h"
#include "MaitaFallingState.h"

#include "SpriteComponent.h"
#include "MaitaComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>


class MaitaJumpState final : public MaitaState
{
public:
	explicit MaitaJumpState(dae::GameObject* pEnemy, MaitaComponent* pEnemyComp, bool isAngry) :
		MaitaState{},
		m_IsAngry{ isAngry },
		m_TimeToJump{ isAngry ? 0.5f : 1.f },
		m_pEnemy{ pEnemy },
		m_pEnemyComp{ pEnemyComp },
		m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
		m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
		m_pCollisionComp{ pEnemy->GetComponent<dae::CollisionComponent>() }
	{};
	virtual ~MaitaJumpState() = default;

	MaitaJumpState(const MaitaJumpState&) = delete;
	MaitaJumpState(MaitaJumpState&&) noexcept = delete;
	MaitaJumpState& operator= (const MaitaJumpState&) = delete;
	MaitaJumpState& operator= (MaitaJumpState&&) noexcept = delete;

	virtual std::unique_ptr<MaitaState> Update() override
	{
		/*dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
		if (pCollidedObject)
		{
			if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
			{
				return std::make_unique<MaitaCaughtState>(m_pEnemy, pCollidedObject);
			}
		}*/

		auto deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
		m_TimeBeforeJump += deltaTime;
		m_TimeBeforeFlip += deltaTime;

		if (m_CheckIfLanded)
		{
			if (m_pPhysicsComp->GetVelocity().y > 0.f) return std::make_unique<MaitaFallingState>(m_pEnemy, m_pEnemyComp, m_IsAngry);
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
	virtual void OnEnter() override
	{

		m_pSpriteComp->SetCol(0);
		m_pSpriteComp->SetUpdate(false);

		m_pSpriteComp->Flip();

		m_pPhysicsComp->SetVelocityX(0);
	}
	virtual void OnExit() override
	{

		m_pSpriteComp->SetUpdate(true);

	}

private:

	bool m_IsAngry;
	bool m_CheckIfLanded{ false };

	float m_TimeToJump;
	float m_TimeBeforeJump{};
	float m_TimeBeforeFlip{};

	const float m_JumpVelocity{ -160.f };


	dae::GameObject* m_pEnemy;
	MaitaComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
	dae::CollisionComponent* m_pCollisionComp;
};


#endif // !MAITAJUMPSTATE_H