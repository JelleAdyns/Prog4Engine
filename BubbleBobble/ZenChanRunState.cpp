#include "ZenChanRunState.h"
#include "ZenChanFallingState.h"
#include "ZenChanJumpState.h"
#include <GameTime.h>

ZenChanRunState::ZenChanRunState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp) :
	ZenChanState{},
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pSpriteComp{ pEnemy->GetComponent<SpriteComponent>() },
	m_pWallCheckingComp{ pEnemy->GetComponent<WallCheckingComponent>() },
	m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() }
{}
std::unique_ptr<EnemyState> ZenChanRunState::Update()
{

	if (m_HasToJump)
	{
		return std::make_unique<ZenChanJumpState>(m_pEnemy, m_pEnemyComp);
	}

	if (!m_pFloorCheckingComp->IsOnGround()) return std::make_unique<ZenChanFallingState>(m_pEnemy, m_pEnemyComp);

	if (m_pWallCheckingComp->CollidingWithLeft()) m_pPhysicsComp->SetVelocityX(m_pEnemyComp->GetSpeed());
	if (m_pWallCheckingComp->CollidingWithRight()) m_pPhysicsComp->SetVelocityX(-m_pEnemyComp->GetSpeed());

	return nullptr;
}
void ZenChanRunState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pEnemyComp->GetPlayerSubjects())
	{
		pSubject->AddObserver(this);
	}

	m_pSpriteComp->SetRow(0);
}
void ZenChanRunState::OnExit()
{
	for (auto& pSubject : m_pVecObservedSpriteSubjects)
	{
		pSubject->RemoveObserver(this);
	}
}

void ZenChanRunState::Notify(PlayerComponent* pSubject)
{
	auto subjectPos = pSubject->GetPos();
	auto enemyPos = m_pEnemy->GetWorldPosition();
	if (subjectPos.y < enemyPos.y)
	{
		float subjectMiddleX{ subjectPos.x + pSubject->GetDestRectSize().x / 2 };
		if (subjectMiddleX > enemyPos.x && subjectMiddleX < enemyPos.x + m_pSpriteComp->GetDestRectSize().x)
		{
			m_HasToJump = true;
		}
	}
	
}

void ZenChanRunState::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.push_back(pSubject);
}
