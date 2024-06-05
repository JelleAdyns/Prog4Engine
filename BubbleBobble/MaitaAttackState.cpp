#include "MaitaAttackState.h"
#include "MaitaRunState.h"
#include "MaitaCaughtState.h"
#include "EnemyComponent.h"
#include "BubbleComponent.h"
#include "Spawners.h"
#include "CollisionTags.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>
#include <algorithm>

MaitaAttackState::MaitaAttackState(dae::GameObject* pEnemy, EnemyComponent* pEnemyComp, bool isAngry):
	MaitaState{pEnemy},
	m_pEnemy{pEnemy},
	m_pEnemyComp{pEnemyComp},
	m_IsAngry{isAngry},
	m_pPhysicsComp{pEnemy->GetComponent<dae::PhysicsComponent>()},
	m_pCollisionComp{pEnemy->GetComponent<dae::CollisionComponent>()},
	m_pSpriteComp{pEnemy->GetComponent<SpriteComponent>()}
{
	m_pSpriteComp->AddObserver(this);
}

MaitaAttackState::~MaitaAttackState()
{
	for (dae::Subject<SpriteComponent>* pSpriteSubject : m_pVecObservedSpriteSubjects)
	{
		if (pSpriteSubject) pSpriteSubject->RemoveObserver(this);
	}
}

std::unique_ptr<EnemyState> MaitaAttackState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<MaitaCaughtState>(m_pEnemy, pCollidedObject);
		}
	}
	if (m_Done) return std::make_unique<MaitaRunState>(m_pEnemy, m_pEnemyComp, m_IsAngry);
	return nullptr;
}

void MaitaAttackState::OnEnter()
{
	m_pSpriteComp->SetCol(0);
	m_pSpriteComp->AddRows(m_AttackInfo.rowNumber);
	m_pSpriteComp->SetFrameTime(m_IsAngry ? m_AttackInfo.frameTime * 0.75f : m_AttackInfo.frameTime);
	m_pSpriteComp->SetRowUpdate(m_AttackInfo.rowUpdate);
	m_pPhysicsComp->SetVelocityX(0.f);
}

void MaitaAttackState::OnExit()
{
	m_pSpriteComp->AddRows(-m_AttackInfo.rowNumber);

	auto pos = m_pEnemy->GetWorldPosition();
	if (!m_pSpriteComp->IsLookingLeft())
	{
		pos.x += m_pSpriteComp->GetDestRectSize().x - EnemyComponent::GetMaitaOffset();
	}

	spawners::SpawnProjectile(pos, m_pSpriteComp->IsLookingLeft());
}


void MaitaAttackState::Notify(SpriteComponent*)
{
	m_Done = true;
}

void MaitaAttackState::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.emplace_back(pSubject);
}

void MaitaAttackState::SetSubjectPointersInvalid(dae::Subject<SpriteComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSpriteSubjects.begin(), m_pVecObservedSpriteSubjects.end(), pSubject);
	if (pos != m_pVecObservedSpriteSubjects.cend())
	{
		m_pVecObservedSpriteSubjects.erase(pos);
	}
}
