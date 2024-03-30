#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H

#include <string>
#include "Component.h"
namespace dae
{
	class GameObject;
	class TextComponent;
	class FPSComponent final : public Component
	{
	public:
		explicit FPSComponent(GameObject* pOwner);
		virtual ~FPSComponent() = default;

		FPSComponent(const FPSComponent&) = delete;
		FPSComponent(FPSComponent&&) noexcept = delete;
		FPSComponent& operator= (const FPSComponent&) = delete;
		FPSComponent& operator= (FPSComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;


	private:
		float m_Fps;
		float m_UpdateTextDelay;
		int m_Count;

		TextComponent* m_pTextComponent;
	};

}

#endif // !FPSCOMPONENT_H