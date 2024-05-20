#ifndef BUTTONHANDLERCOMPONENT_H
#define BUTTONHANDLERCOMPONENT_H

#include <Component.h>
#include <Command.h>
#include <memory>
#include <vector>
#include "ButtonComponent.h"

class ButtonHandlerComponent final : public dae::Component
{
public:
	explicit ButtonHandlerComponent(dae::GameObject* pOwner);
	virtual ~ButtonHandlerComponent() = default;

	ButtonHandlerComponent(const ButtonHandlerComponent&) = delete;
	ButtonHandlerComponent(ButtonHandlerComponent&&) noexcept = delete;
	ButtonHandlerComponent& operator= (const ButtonHandlerComponent&) = delete;
	ButtonHandlerComponent& operator= (ButtonHandlerComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;
	
	void Activate();
	void AddButton(ButtonComponent* newButton);
	void SelectNext();
	void SelectPrevious();
	ButtonComponent* GetSelectedButton() const;

private:

	bool m_ButtonIsActivated;
	int m_SelectedIndex;

	std::vector<ButtonComponent*> m_pVecButtons{};

};
#endif // !BUTTONHANDLERCOMPONENT_H
