#include "LivesUIComponent.h"
#include "TextComponent.h"
#include "PlayerComponent.h"
#include <GameObject.h>
#include <algorithm>

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

void LivesUIComponent::SetSubjectPointersInvalid(dae::Subject<PlayerComponent>* pSubject)
{
    auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pSubject);
    if (pos != m_pVecObservedSubjects.cend())
    {
        m_pVecObservedSubjects.erase(pos);
    }
}

void LivesUIComponent::AddLifeObjct(dae::GameObject* pLife)
{
    m_pLives.push_back(pLife);
}
