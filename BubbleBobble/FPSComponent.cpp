#include "FPSComponent.h"
#include <sstream>
#include <iomanip>
#include "GameObject.h"
#include "TextComponent.h"

FPSComponent::FPSComponent(dae::GameObject* pOwner):
	dae::Component{ pOwner }, m_Fps{}, m_UpdateTextDelay{}, m_Count{}, m_pTextComponent{nullptr}
{
}

void FPSComponent::Start()
{
	if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();
}

void FPSComponent::Update()
{

	m_UpdateTextDelay += dae::GameTime::GetInstance().GetDeltaTime();
	++m_Count;
	if (m_UpdateTextDelay >= 0.5f)
	{
		m_Fps = m_Count / m_UpdateTextDelay;
		std::stringstream fps;
		fps << std::fixed << std::setprecision(1) << m_Fps << " FPS";
			
		m_pTextComponent->SetText(fps.str());
		m_UpdateTextDelay = 0.f;
		m_Count = 0;
	}
}

void FPSComponent::PrepareImGuiRender(){}
