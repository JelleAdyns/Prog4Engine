#include "LivesUIComponent.h"
#include "TextComponent.h"
#include "PlayerComponent.h"
#include <GameObject.h>

LivesUIComponent::LivesUIComponent(dae::GameObject* pOwner) :
	dae::Component{pOwner},
	dae::Observer<PlayerComponent>{},
    m_pVecObservedSubjects{}
{
}
LivesUIComponent::~LivesUIComponent()
{
    for (auto& pSubject : m_pVecObservedSubjects)
    {
        if(pSubject) pSubject->RemoveObserver(this);
    }
}
void LivesUIComponent::Start()
{

}
void LivesUIComponent::Update()
{
	
}

void LivesUIComponent::PrepareImGuiRender()
{
}

void LivesUIComponent::Notify(PlayerComponent*)
{
    m_pLives.back()->MarkDead();
    m_pLives.pop_back();
}

void LivesUIComponent::AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject)
{
    m_pVecObservedSubjects.emplace_back( pSubject );
}

void LivesUIComponent::SetSubjectPointersInvalid()
{
    for (auto& pSubject : m_pVecObservedSubjects)
    {
        pSubject = nullptr;
    }
}

void LivesUIComponent::AddLifeObjct(dae::GameObject* pLife)
{
    m_pLives.push_back(pLife);
}
