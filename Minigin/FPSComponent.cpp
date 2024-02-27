#include "FPSComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "TextComponent.h"

namespace dae
{
	FPSComponent::FPSComponent(GameObject* pOwner):
		Component{ pOwner }, m_Fps{}, m_UpdateTextDelay{}, m_Count{}
	{
		m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
	}


	void FPSComponent::Update()
	{
		m_UpdateTextDelay += GameTime::GetInstance().GetDeltaTime();
		++m_Count;
		if (m_UpdateTextDelay >= 0.5f)
		{
			m_Fps = m_Count / m_UpdateTextDelay;
			m_pTextComponent->SetText(GetFpsString() + " FPS");
			m_UpdateTextDelay = 0.f;
			m_Count = 0;
		}
	}

	std::string FPSComponent::GetFpsString() const
	{
		return std::to_string(m_Fps);
	}
}
