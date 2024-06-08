#ifndef INITIALSCOMPONENT_H
#define INITIALSCOMPONENT_H

#include <string>
#include <Subject.h>
#include "Component.h"

namespace dae
{
	class GameObject;
	class TextComponent;
}

class InitialsComponent final : public dae::Component
{
public:
	explicit InitialsComponent(dae::GameObject* pOwner, dae::Observer<InitialsComponent>* pObserver);
	virtual ~InitialsComponent() = default;

	InitialsComponent(const InitialsComponent&) = delete;
	InitialsComponent(InitialsComponent&&) noexcept = delete;
	InitialsComponent& operator= (const InitialsComponent&) = delete;
	InitialsComponent& operator= (InitialsComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void ConfirmCharacter();
	void AdvanceCharacter(bool proceedForward);

	void GetInitials(std::string& initials) const { initials = m_Characters; };

private:
	
	static constexpr int m_MaxChars{ 3 };
	char m_Characters[m_MaxChars]{'.'};

	int m_ActiveCharacter{};
	bool m_Notify{};

	dae::TextComponent* m_pTextComponent;
	std::unique_ptr<dae::Subject<InitialsComponent>> m_pNameConfirmed;
};

#endif // !INITIALSCOMPONENT_H