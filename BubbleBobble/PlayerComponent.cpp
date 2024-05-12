#include "PlayerComponent.h"
#include "IdleState.h"
#include "SpriteComponent.h"
#include <PhysicsComponent.h>
#include <KeyState.h>
#include <InputCommandBinder.h>

uint8_t PlayerComponent::m_NrOfPlayers{};

PlayerComponent::PlayerComponent(dae::GameObject* pOwner):
	dae::Component{pOwner},
	m_pCurrState{}
{
	m_PlayerIndex = m_NrOfPlayers;
	++m_NrOfPlayers;

}

PlayerComponent::~PlayerComponent()
{
	--m_NrOfPlayers;
}

void PlayerComponent::Update()
{
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();

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
	m_pVecObservedSubjects.emplace_back(pSubject);
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

void PlayerComponent::UpdateStates()
{
	if (!m_pCurrState)
	{
		m_pCurrState = std::make_unique<IdleState>(GetOwner(), this);
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
