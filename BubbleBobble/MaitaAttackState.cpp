#include "MaitaAttackState.h"
#include "MaitaRunState.h"
#include "MaitaCaughtState.h"
#include "MaitaComponent.h"
#include "Spawners.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <GameObject.h>

MaitaAttackState::MaitaAttackState(dae::GameObject* pMaita, MaitaComponent* pMaitaComp, bool isAngry):
	MaitaState{},
	m_pMaita{pMaita},
	m_pMaitaComp{pMaitaComp},
	m_IsAngry{isAngry},
	m_pPhysicsComp{pMaita->GetComponent<dae::PhysicsComponent>()},
	m_pCollisionComp{pMaita->GetComponent<dae::CollisionComponent>()},
	m_pSpriteComp{pMaita->GetComponent<SpriteComponent>()}
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

std::unique_ptr<MaitaState> MaitaAttackState::Update()
{
	dae::GameObject* pCollidedObject = m_pCollisionComp->CheckForCollision(collisionTags::bubbleTag);
	if (pCollidedObject)
	{
		if (!pCollidedObject->GetComponent<BubbleComponent>()->IsOccupied())
		{
			return std::make_unique<MaitaCaughtState>(m_pMaita, pCollidedObject);
		}
	}
	if (m_Done) return std::make_unique<MaitaRunState>(m_pMaita, m_pMaitaComp, m_IsAngry);
	return nullptr;
}

void MaitaAttackState::OnEnter()
{
	m_pSpriteComp->SetCol(0);
	m_pSpriteComp->AddRows(m_AttackInfo.rowNumber);
	m_pSpriteComp->SetFrameTime(m_IsAngry ? m_AttackInfo.frameTime * 0.5f : m_AttackInfo.frameTime);
	m_pSpriteComp->SetRowUpdate(m_AttackInfo.rowUpdate);
	m_pPhysicsComp->SetVelocityX(0.f);
}

void MaitaAttackState::OnExit()
{
	m_pSpriteComp->AddRows(-m_AttackInfo.rowNumber);

	auto pos = m_pMaita->GetWorldPosition();
	if (!m_pSpriteComp->IsLookingLeft())
	{
		pos.x += m_pSpriteComp->GetDestRectSize().x - MaitaComponent::GetMaitaOffset();
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

void MaitaAttackState::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSpriteSubjects)
	{
		pSubject = nullptr;
	}
}
