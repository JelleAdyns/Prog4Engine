#ifndef BUTTONCOMPONENT_H
#define BUTTONCOMPONENT_H

#include <Component.h>
#include <Command.h>
#include <memory>

class ButtonComponent final : public dae::Component
{
public:
	explicit ButtonComponent(dae::GameObject* pOwner, std::unique_ptr<dae::Command>& pCommand);
	virtual ~ButtonComponent() = default;

	ButtonComponent(const ButtonComponent&) = delete;
	ButtonComponent(ButtonComponent&&) noexcept = delete;
	ButtonComponent& operator= (const ButtonComponent&) = delete;
	ButtonComponent& operator= (ButtonComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void Activate() const;
	void Select(bool select);
	bool IsSelected() const;
	glm::vec2 GetPos() const;
private:

	bool m_IsSelected;
	std::unique_ptr<dae::Command> m_pCommandToExecute;

};
#endif // !BUTTONCOMPONENT_H
