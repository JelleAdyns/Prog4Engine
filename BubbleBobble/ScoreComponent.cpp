#include "ScoreComponent.h"
#include "SpriteComponent.h"
#include <GameObject.h>
#include <Gametime.h>

ScoreComponent::ScoreComponent(dae::GameObject* pOwner, int score, PlayerComponent::PlayerType playerType):
	dae::Component{pOwner},
	m_Score{ score },
	m_PlayerWhoScored{playerType},
	m_pSpriteComp{}
{

}

void ScoreComponent::Start()
{
	if (!m_pSpriteComp) m_pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();

	switch (m_PlayerWhoScored)
	{
	case PlayerComponent::PlayerType::Green:
		m_pSpriteComp->SetRow(0);
		break;
	case PlayerComponent::PlayerType::Blue:
		m_pSpriteComp->SetRow(1);
		break;
	}

	switch (m_Score)
	{
	case 100:
		m_pSpriteComp->SetCol(0);
		break;
	case 200:
		m_pSpriteComp->SetCol(1);
		break;
	}
}

void ScoreComponent::Update()
{
	auto deltatime = dae::GameTime::GetInstance().GetDeltaTime();

	m_Timer += deltatime;

	if (m_Timer >= m_MaxTimeAlive) GetOwner()->MarkDead();

	static constexpr int floatSpeed{ 20 };
	auto localPos = GetOwner()->GetLocalPosition();
	GetOwner()->SetLocalPos(localPos.x, localPos.y -= floatSpeed * deltatime);

}

void ScoreComponent::PrepareImGuiRender()
{
}
