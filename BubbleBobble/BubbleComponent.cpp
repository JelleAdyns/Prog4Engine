#include "BubbleComponent.h"
#include "EnemyComponent.h"

BubbleComponent::BubbleComponent(dae::GameObject* pOwner):
	dae::Component{pOwner}
{
}

void BubbleComponent::Start()
{
}

void BubbleComponent::Update()
{
}

void BubbleComponent::PrepareImGuiRender()
{
}

void BubbleComponent::Notify(EnemyComponent*)
{
}

void BubbleComponent::AddSubjectPointer(dae::Subject<EnemyComponent>* pSubject)
{
	m_pVecObservedSpriteSubjects.push_back(pSubject);
}
