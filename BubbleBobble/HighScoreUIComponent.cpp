#include "HighScoreUIComponent.h"
#include "ScoreUIComponent.h"
#include "HighScoreHandling.h"
#include <TextComponent.h>

HighScoreUIComponent::HighScoreUIComponent(dae::GameObject* pOwner):
    dae::Component{pOwner}
{
    m_CurrentHighScore = highScoreHandling::GetFirstScore().score;
}

HighScoreUIComponent::~HighScoreUIComponent()
{
    for (auto& pSubject : m_pVecObservedSubjects)
    {
        if (pSubject) pSubject->RemoveObserver(this);
    }
}

void HighScoreUIComponent::Start()
{
    if(!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();

    m_pTextComponent->SetText(std::to_string(m_CurrentHighScore));
}

void HighScoreUIComponent::Update()
{
}

void HighScoreUIComponent::PrepareImGuiRender()
{
}

void HighScoreUIComponent::Notify(ScoreUIComponent* pSubject)
{
    if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();

    if (pSubject->GetScore() > m_CurrentHighScore)
        m_CurrentHighScore = pSubject->GetScore();

    m_pTextComponent->SetText(std::to_string(m_CurrentHighScore));
}

void HighScoreUIComponent::AddSubjectPointer(dae::Subject<ScoreUIComponent>* pSubject)
{
    m_pVecObservedSubjects.emplace_back(pSubject);
}

void HighScoreUIComponent::SetSubjectPointersInvalid(dae::Subject<ScoreUIComponent>* pSubject)
{
    auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pSubject);
    if (pos != m_pVecObservedSubjects.cend())
    {
        m_pVecObservedSubjects.erase(pos);
    }
}
