#pragma once
#include <string>
#include "Component.h"
namespace dae
{
	class GameObject;
	class FPSComponent final : public Component
	{
	public:
		explicit FPSComponent(const std::shared_ptr<GameObject>& pOwner);
		virtual ~FPSComponent() = default;

		FPSComponent(const FPSComponent&) = delete;
		FPSComponent(FPSComponent&&) noexcept = delete;
		FPSComponent& operator= (const FPSComponent&) = delete;
		FPSComponent& operator= (FPSComponent&&) noexcept = delete;

		virtual void Update() override;

		std::string GetFpsString() const;

	private:
		float m_Fps;
		float m_UpdateTextDelay;
	};

}

