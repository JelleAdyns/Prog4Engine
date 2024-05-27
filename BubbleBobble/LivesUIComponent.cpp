#include "LivesUIComponent.h"
#include "TextComponent.h"
#include "LivesComponent.h"
#include "GameObject.h"

LivesUIComponent::LivesUIComponent(dae::GameObject* pOwner) :
	dae::Component{pOwner},
	dae::Observer<LivesComponent>{},
	m_pTextComponent{},
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
    if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();
}
void LivesUIComponent::Update()
{
	
}

void LivesUIComponent::PrepareImGuiRender()
{
}

void LivesUIComponent::Notify(LivesComponent* pSubject)
{
	m_pTextComponent->SetText("Remaining lives: " + std::to_string(pSubject->GetNrOfLives()));
}

void LivesUIComponent::AddSubjectPointer(dae::Subject<LivesComponent>* pSubject)
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
