//#include "ZenChanComponent.h"
//#include "PlayerComponent.h"
//#include <PhysicsComponent.h>
//#include <GameObject.h>
//
//ZenChanComponent::ZenChanComponent(dae::GameObject* pOwner) :
//	dae::Component{ pOwner },
//	m_pCurrState{},
//	m_pPhysicsComp{},
//	m_pSpriteComp{},
//	m_SubjectsForState{}
//{
//}
//
//void ZenChanComponent::Start()
//{
//	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
//	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();
//}
//
//void ZenChanComponent::Update()
//{
//
//	UpdateStates();
//
//	if (m_pPhysicsComp->GetVelocity().x < 0) m_pSpriteComp->LookLeft(true);
//	if (m_pPhysicsComp->GetVelocity().x > 0) m_pSpriteComp->LookLeft(false);
//
//}
//
//void ZenChanComponent::PrepareImGuiRender()
//{
//}
//
//void ZenChanComponent::AddPlayerObserver(PlayerComponent* pSubject)
//{
//	if(pSubject) m_SubjectsForState.push_back(pSubject->GetSubject());
//}
//
//void ZenChanComponent::UpdateStates()
//{
//	if (!m_pCurrState)
//	{
//		m_pCurrState = std::make_unique<ZenChanRunState>(GetOwner(), this);
//		m_pCurrState->OnEnter();
//	}
//
//	auto pNewState = m_pCurrState->Update();
//
//	if (pNewState)
//	{
//		m_pCurrState->OnExit();
//		m_pCurrState = std::move(pNewState);
//		m_pCurrState->OnEnter();
//	}
//}
