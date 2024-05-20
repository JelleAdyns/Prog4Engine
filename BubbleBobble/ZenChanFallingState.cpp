#include "ZenChanFallingState.h"
#include "ZenChanRunState.h"

ZenChanFallingState::ZenChanFallingState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp) :
	ZenChanState{},
	m_pEnemy{ pEnemy },
	m_pEnemyComp{ pEnemyComp },
	m_pPhysicsComp{ pEnemy->GetComponent<dae::PhysicsComponent>() },
	m_pFloorCheckingComp{ pEnemy->GetComponent<FloorCheckingComponent>() }
{}


std::unique_ptr<EnemyState> ZenChanFallingState::Update()
{
	/*if (m_pEnemyComp->IsHit()) return std::make_unique<HitState>(m_pEnemy, m_pEnemyComp);*/
	if (m_pEnemy->GetWorldPosition().y > dae::Minigin::GetWindowSize().y) m_pEnemy->SetLocalPos(m_pEnemy->GetLocalPosition().x, -50);
	if (m_pFloorCheckingComp->IsOnGround())
	{
		return std::make_unique<ZenChanRunState>(m_pEnemy, m_pEnemyComp);
	}

	return nullptr;
}
void ZenChanFallingState::OnEnter()
{
	for (dae::Subject<PlayerComponent>* pSubject : m_pEnemyComp->GetPlayerSubjects())
	{
		pSubject->AddObserver(this);
	}
	//m_pEnemy->GetComponent<SpriteComponent>()->SetRow(3);
	m_pPhysicsComp->SetVelocityX(0);
}
void ZenChanFallingState::OnExit()
{

	if (m_PlayerXPos < m_pEnemy->GetWorldPosition().x) m_pPhysicsComp->SetVelocityX(-m_pEnemyComp->GetSpeed());
	else m_pPhysicsComp->SetVelocityX(m_pEnemyComp->GetSpeed());
	

	for (auto& pSubject : m_pVecObservedSpriteSubjects)
	{
		pSubject->RemoveObserver(this);
	}
}

void ZenChanFallingState::Notify(PlayerComponent* pSubject)
{
	auto subjectPos = pSubject->GetPos();
	m_PlayerXPos = subjectPos.x;
}

void ZenChanFallingState::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.push_back(pSubject);
}
