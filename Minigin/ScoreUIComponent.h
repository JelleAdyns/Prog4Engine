#ifndef SCOREUICOMPONENT_H
#define SCOREUICOMPONENT_H

#include "Observer.h"
#include "Component.h"
#include <string>

namespace dae
{
	class TextComponent;
	class GameObject;
	class PickUpComponent;
	class ScoreUIComponent final : public Component, public Observer<PickUpComponent>
	{
	public:
		explicit ScoreUIComponent(GameObject* pOwner);
		virtual ~ScoreUIComponent() = default;

		ScoreUIComponent(const ScoreUIComponent&) = delete;
		ScoreUIComponent(ScoreUIComponent&&) noexcept = delete;
		ScoreUIComponent& operator= (const ScoreUIComponent&) = delete;
		ScoreUIComponent& operator= (ScoreUIComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		virtual void Notify(PickUpComponent* pSubject) override;
	private:
		int m_Score;
		TextComponent* m_pTextComponent;
	};

}

#endif // !SCOREUICOMPONENT_H