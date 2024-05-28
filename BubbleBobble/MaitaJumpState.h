#ifndef MAITAJUMPSTATE_H
#define MAITAJUMPSTATE_H

#include "MaitaState.h"
#include "MaitaFallingState.h"
#include "MaitaCaughtState.h"

#include "SpriteComponent.h"
#include "MaitaComponent.h"
#include "BubbleComponent.h"
#include "CollisionTags.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <Minigin.h>
#include <GameTime.h>


class MaitaJumpState final : public MaitaState
{
public:
	explicit MaitaJumpState(dae::GameObject* pMaita, MaitaComponent* pMaitaComp, bool isAngry);
	virtual ~MaitaJumpState() = default;

	MaitaJumpState(const MaitaJumpState&) = delete;
	MaitaJumpState(MaitaJumpState&&) noexcept = delete;
	MaitaJumpState& operator= (const MaitaJumpState&) = delete;
	MaitaJumpState& operator= (MaitaJumpState&&) noexcept = delete;

	virtual std::unique_ptr<MaitaState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:

	bool m_IsAngry;
	bool m_CheckIfLanded{ false };

	float m_TimeToJump;
	float m_TimeBeforeJump{};
	float m_TimeBeforeFlip{};

	const float m_JumpVelocity{ -160.f };


	dae::GameObject* m_pMaita;
	MaitaComponent* m_pMaitaComp;
	dae::PhysicsComponent* m_pPhysicsComp;
	SpriteComponent* m_pSpriteComp;
	dae::CollisionComponent* m_pCollisionComp;
};


#endif // !MAITAJUMPSTATE_H