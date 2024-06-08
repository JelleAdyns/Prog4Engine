#include "PickUpComponent.h"
#include "InventoryComponent.h"
#include "CollisionTags.h"
#include "spawners.h"
#include <CollisionComponent.h>
#include <RenderComponent.h>
#include <GameObject.h>
#include <GameTime.h>
#include <algorithm>
#include <AudioLocator.h>
#include "Game.h"

PickUpComponent::PickUpComponent(dae::GameObject* pOwner, PickUpComponent::PickUpType pickUpType):
	dae::Component{pOwner},
	m_PickUpType{pickUpType},
	m_pCollisionComp{},
	m_pSpriteComp{}
{
	dae::AudioLocator::GetAudioService().AddSound("Sounds/Pickup.wav", static_cast<dae::SoundID>(Game::SoundEvent::PickUp));
}

PickUpComponent::~PickUpComponent()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		if (pSubject) pSubject->RemoveObserver(this);
	}
}

void PickUpComponent::Start()
{
	if (!m_pCollisionComp) m_pCollisionComp = GetOwner()->GetComponent<dae::CollisionComponent>();
	if (!m_pRenderComp) m_pRenderComp = GetOwner()->GetComponent<dae::RenderComponent>();
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();

	switch (m_PickUpType)
	{
	case PickUpComponent::PickUpType::Melon:
		m_pSpriteComp->SetCol(0);
		break;
	case PickUpComponent::PickUpType::Fries:
		m_pSpriteComp->SetCol(1);
		break;
	}

}
 
void PickUpComponent::Update()
{
	HandleTimers();

	if(m_Timer < m_MaxTimeAlive)
	{
		dae::GameObject* pPlayer = m_pCollisionComp->CheckForCollision(collisionTags::playerTag);
		if (pPlayer)
		{
			pPlayer->GetComponent<InventoryComponent>()->AddItem(m_PickUpType, GetOwner()->GetWorldPosition());
			GetOwner()->MarkDead();
			dae::AudioLocator::GetAudioService().PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::PickUp), 120, false);
		}
	}
}

void PickUpComponent::PrepareImGuiRender()
{
}

void PickUpComponent::Notify(SpriteComponent*)
{
	GetOwner()->MarkDead();
}

void PickUpComponent::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSubjects.emplace_back(pSubject);
}

void PickUpComponent::SetSubjectPointersInvalid(dae::Subject<SpriteComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pSubject);
	if (pos != m_pVecObservedSubjects.cend())
	{
		m_pVecObservedSubjects.erase(pos);
	}
}


PickUpComponent::PickUpType PickUpComponent::GetPickUpType() const
{
	return m_PickUpType;
}

void PickUpComponent::HandleTimers()
{
	if(m_Timer < m_MaxTimeAlive)
	{
		auto deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
		m_Timer += deltaTime;

		if (m_Timer >= m_TimeToFlicker)
		{
			m_FlickerTimer += deltaTime;
			if (m_FlickerTimer >= m_FlickerDelay)
			{
				m_pRenderComp->ToggleNeedToRender();
				m_FlickerTimer = 0.f;
			}
		}
		if (m_Timer >= m_MaxTimeAlive)
		{
			m_pSpriteComp->SetCol(2);
			m_pSpriteComp->SetUpdate(true);
		}
	}

}
