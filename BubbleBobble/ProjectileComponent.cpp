#include "ProjectileComponent.h"
#include "WallCheckingComponent.h"
#include <GameObject.h>
#include <PhysicsComponent.h>
#include <CollisionComponent.h>
#include <algorithm>
#include <AudioLocator.h>
#include "Game.h"


ProjectileComponent::ProjectileComponent(dae::GameObject* pOwner, bool left) :
	dae::Component{ pOwner },
	m_Velocity{left ? -m_Speed : m_Speed},
	m_pSpriteComp{},
	m_pWallComp{},
	m_pPhysicsComp{}
{
	dae::AudioLocator::GetAudioService().AddSound("Sounds/Boulder.wav", static_cast<dae::SoundID>(Game::SoundEvent::Boulder));
}

ProjectileComponent::~ProjectileComponent()
{
	for (auto& pSubject : m_pVecObservedSubjects)
	{
		if (pSubject) pSubject->RemoveObserver(this);
	}
}

void ProjectileComponent::Start()
{
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();
	if (!m_pWallComp) m_pWallComp = GetOwner()->GetComponent<WallCheckingComponent>();
	if (!m_pPhysicsComp) m_pPhysicsComp = GetOwner()->GetComponent<dae::PhysicsComponent>();
	m_pPhysicsComp->SetVelocityX(m_Velocity);
	m_pPhysicsComp->StopGravity();
	m_pSpriteComp->AddObserver(this);
}

void ProjectileComponent::Update()
{
	if(!m_IsBroken)
	{
		if (m_pWallComp->CollidingWithLeft() || m_pWallComp->CollidingWithRight())
		{
			m_IsBroken = true;
			GetOwner()->GetComponent<dae::CollisionComponent>()->SetCollision(false);
			m_pPhysicsComp->SetVelocityX(0.f);
			m_pSpriteComp->AddRows(1);
			m_pSpriteComp->SetCol(0);
			m_pSpriteComp->SetFrameTime(0.2f);
			dae::AudioLocator::GetAudioService().PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::Boulder), 120, false);
		}
	}
}

void ProjectileComponent::PrepareImGuiRender()
{
}

void ProjectileComponent::Notify(SpriteComponent*)
{
	if (m_IsBroken)
	{
		
		GetOwner()->MarkDead();

	}
}

void ProjectileComponent::AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject)
{
	m_pVecObservedSubjects.emplace_back(pSubject);
}

void ProjectileComponent::SetSubjectPointersInvalid(dae::Subject<SpriteComponent>* pSubject)
{
	auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pSubject);
	if (pos != m_pVecObservedSubjects.cend())
	{
		m_pVecObservedSubjects.erase(pos);
	}
}
