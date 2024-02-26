#include "FPSComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "TextComponent.h"

namespace dae
{
	FPSComponent::FPSComponent(const std::shared_ptr<GameObject>& pOwner):
		Component{pOwner}, m_Fps{}, m_UpdateTextDelay{}
	{}


	void FPSComponent::Update()
	{
		m_Fps = 1.f / GameTime::GetInstance().GetDeltaTime();
		m_UpdateTextDelay += GameTime::GetInstance().GetDeltaTime();
		if (m_UpdateTextDelay >= 0.5f)
		{
			m_pOwner.lock()->GetComponent<TextComponent>()->SetText(GetFpsString() + " FPS");
			m_UpdateTextDelay = 0.f;
		}
			
		
	}

	std::string FPSComponent::GetFpsString() const
	{
		return std::to_string(m_Fps);
	}
}
