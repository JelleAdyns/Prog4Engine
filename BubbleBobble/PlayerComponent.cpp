#include "PlayerComponent.h"
#include "IdleState.h"
#include "HitState.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <KeyState.h>
#include <GameTime.h>
#include <Minigin.h>
#include <InputCommandBinder.h>



PlayerComponent::PlayerComponent(dae::GameObject* pOwner):
	dae::Component{pOwner},
	m_pCurrState{},
	m_pPosChecked{std::make_unique<dae::Subject<PlayerComponent>>()}
{
	Respawn();
}

PlayerComponent::~PlayerComponent()
{

	
	for (dae::Subject<SpriteComponent>* pSpriteSubject : m_pVecObservedSpriteSubjects)
	{
		pSpriteSubject->RemoveObserver(this);
	}

	for (dae::Subject<EnemyComponent>* pEnemySubject : m_pVecObservedEnemySubjects)
	{
		pEnemySubject->RemoveObserver(this);
	}
}

void PlayerComponent::Start()
{
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();
	if (!m_pMovementComp) m_pMovementComp = GetOwner()->GetComponent<MovementComponent>();
}

void PlayerComponent::Update()
{
	
	if (m_IsInvincible)
	{
		m_InvincibilityTimer += dae::GameTime::GetInstance().GetDeltaTime();

		if(m_InvincibilityTimer >= m_InvincibilityMaxTime)
		{
			m_InvincibilityTimer = 0.f;
			m_IsInvincible = false;
		}
	}
	m_pPosChecked->NotifyObservers(this);
	UpdateStates();

	if (m_pPhysicsComp->GetVelocity().x < 0) m_pSpriteComp->LookLeft(true);
	if (m_pPhysicsComp->GetVelocity().x > 0) m_pSpriteComp->LookLeft(false);

	m_pPhysicsComp->SetVelocityX(0);
}

void PlayerComponent::PrepareImGuiRender()
{
}

void PlayerComponent::Notify(SpriteComponent* pSubject)
{
	if (m_IsShooting)
	{
		m_IsShooting = false;
		pSubject->SetStartRow(0);
		pSubject->SetFrameTime(0.1f);
	}
}

void PlayerComponent::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.emplace_back(pSubject);
}

void PlayerComponent::Notify(EnemyComponent*)
{
	if(!m_IsHit && !m_IsInvincible)
	{
		m_IsHit = true;
		m_pPhysicsComp->SetVelocityY(0);
		m_pPhysicsComp->SetVelocityX(0);
	}
}
void PlayerComponent::AddSubjectPointer(dae::Subject<EnemyComponent>* pSubject)
{
	m_pVecObservedEnemySubjects.emplace_back(pSubject);
}
void PlayerComponent::Shoot()
{
	if(!m_IsShooting)
	{
		m_IsShooting = true;
		m_pSpriteComp->SetFrameTime(0.2f);
		m_pSpriteComp->SetStartRow(4);
		m_pSpriteComp->SetCol(0);
	}
}

bool PlayerComponent::IsHit() const
{
	return m_IsHit;
}

void PlayerComponent::Respawn()
{
	m_IsHit = false;
	GetOwner()->SetLocalPos(24.f, dae::Minigin::GetWindowSize().y - 40.f);
	m_IsInvincible = true;
}

dae::Subject<PlayerComponent>* PlayerComponent::GetSubject() const
{
	return m_pPosChecked.get();
}

glm::vec2 PlayerComponent::GetPos() const
{
	return GetOwner()->GetWorldPosition();
}


void PlayerComponent::UpdateStates()
{
	if (!m_pCurrState)
	{
		m_pCurrState = std::make_unique<IdleState>(GetOwner(), this, m_pMovementComp);
		m_pCurrState->OnEnter();
	}

	auto newState = m_pCurrState->Update();

	if (newState != nullptr)
	{
		m_pCurrState->OnExit();
		m_pCurrState = std::move(newState);
		m_pCurrState->OnEnter();
	}
}
