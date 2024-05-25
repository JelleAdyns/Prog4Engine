#ifndef ZENCHANJUMPSTATE_H
#define ZENCHANJUMPSTATE_H

#include "ZenChanState.h"

#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include <PhysicsComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>


class ZenChanJumpState final : public ZenChanState
{
public:
	explicit ZenChanJumpState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp) :
		ZenChanState{},
		m_pEnemy{ pEnemy },
		m_pEnemyComp{ pEnemyComp },
		m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
		m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() }
	{};
	virtual ~ZenChanJumpState() = default;

	ZenChanJumpState(const ZenChanJumpState&) = delete;
	ZenChanJumpState(ZenChanJumpState&&) noexcept = delete;
	ZenChanJumpState& operator= (const ZenChanJumpState&) = delete;
	ZenChanJumpState& operator= (ZenChanJumpState&&) noexcept = delete;

	virtual std::unique_ptr<EnemyState> Update() override
	{

		auto deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
		m_TimeBeforeJump += deltaTime;
		m_TimeBeforeFlip += deltaTime;

		if(m_CheckIfLanded)
		{
			if (m_pPhysicsComp->GetVelocity().y > 0.f) return std::make_unique<ZenChanFallingState>(m_pEnemy, m_pEnemyComp);
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

	bool m_CheckIfLanded{false};

	float m_TimeBeforeJump{};
	float m_TimeBeforeFlip{}; 
	float m_TimeToJump{ 1.f };

	const float m_JumpVelocity{ -160.f };


	dae::GameObject* m_pEnemy;
	EnemyComponent* m_pEnemyComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
};


#endif // !ZENCHANJUMPSTATE_H