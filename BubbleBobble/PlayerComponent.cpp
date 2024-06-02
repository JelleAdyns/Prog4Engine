#include "PlayerComponent.h"
#include "IdleState.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include "CollisionTags.h"
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <RenderComponent.h>
#include <GameTime.h>
#include <Minigin.h>


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
		if(pSpriteSubject) pSpriteSubject->RemoveObserver(this);
	}
}

void PlayerComponent::Start()
{
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();
	if (!m_pRenderComp) m_pRenderComp = GetOwner()->GetComponent<dae::RenderComponent>();
	if (!m_pMovementComp) m_pMovementComp = GetOwner()->GetComponent<MovementComponent>();

	if (!m_pCurrState)
	{
		m_pCurrState = std::make_unique<IdleState>(GetOwner(), this, m_pMovementComp);
		m_pCurrState->OnEnter();
	}
}

void PlayerComponent::Update()
{
	m_pCollisionComp->CheckForCollision(collisionTags::enemyTag);
	
	if (m_IsInvincible)
	{
		const auto& deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
		m_InvincibilityTimer += deltaTime;
		m_RenderTimer += deltaTime;

		if (m_RenderTimer >= 0.07f)
		{
			m_pRenderComp->ToggleNeedToRender();
			m_RenderTimer = 0.f;
		}

		if(m_InvincibilityTimer >= m_InvincibilityMaxTime)
		{
			m_InvincibilityTimer = 0.f;
			m_IsInvincible = false;
			m_pRenderComp->SetNeedToRender(true);
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

void PlayerComponent::Notify(SpriteComponent*)
{
	m_pCurrState->StopShooting();
}

void PlayerComponent::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.emplace_back(pSubject);
}

void PlayerComponent::SetSubjectPointersInvalid()
{
	for (auto& pSubject : m_pVecObservedSpriteSubjects)
	{
		pSubject = nullptr;
	}
}

void PlayerComponent::Shoot()
{
	m_pCurrState->Shoot();
}


void PlayerComponent::SetInvincible()
{
	m_IsInvincible = true;
}

bool PlayerComponent::IsInvincible() const
{
	return m_IsInvincible;
}

void PlayerComponent::Respawn()
{
	GetOwner()->SetLocalPos(24.f, dae::Minigin::GetWindowSize().y - 40.f);
}

dae::Subject<PlayerComponent>* PlayerComponent::GetSubject() const
{
	return m_pPosChecked.get();
}

glm::vec2 PlayerComponent::GetPos() const
{
	return GetOwner()->GetWorldPosition();
}

glm::vec2 PlayerComponent::GetDestRectSize() const
{
	return m_pSpriteComp->GetDestRectSize();
}


void PlayerComponent::UpdateStates()
{

	auto newState = m_pCurrState->Update();

	if (newState != nullptr)
	{
		m_pCurrState->OnExit();
		m_pCurrState = std::move(newState);
		m_pCurrState->OnEnter();
	}
}
