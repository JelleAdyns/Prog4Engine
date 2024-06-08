#ifndef INITIALSCOMMANDS_H
#define INITIALSCOMMANDS_H

#include <GameObjectCommand.h>
#include "InitialsComponent.h"

class AdvanceCharacterCommand final : public dae::GameObjectCommand
{
public:
	AdvanceCharacterCommand(dae::GameObject* pObject, bool forward) :
		dae::GameObjectCommand{pObject},
		m_pInitialsComp{pObject->GetComponent<InitialsComponent>()},
		m_Forward{forward}
	{};
	virtual ~AdvanceCharacterCommand() = default;

	AdvanceCharacterCommand(const AdvanceCharacterCommand&) = delete;
	AdvanceCharacterCommand(AdvanceCharacterCommand&&) noexcept = delete;
	AdvanceCharacterCommand& operator= (const AdvanceCharacterCommand&) = delete;
	AdvanceCharacterCommand& operator= (AdvanceCharacterCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		m_pInitialsComp->AdvanceCharacter(m_Forward);
	}
private:
	InitialsComponent* m_pInitialsComp;
	const bool m_Forward;
};

class ConfirmCharacterCommand final : public dae::GameObjectCommand
{
public:
	ConfirmCharacterCommand(dae::GameObject* pObject) :
		dae::GameObjectCommand{ pObject },
		m_pInitialsComp{ pObject->GetComponent<InitialsComponent>() }
	{};
	virtual ~ConfirmCharacterCommand() = default;

	ConfirmCharacterCommand(const ConfirmCharacterCommand&) = delete;
	ConfirmCharacterCommand(ConfirmCharacterCommand&&) noexcept = delete;
	ConfirmCharacterCommand& operator= (const ConfirmCharacterCommand&) = delete;
	ConfirmCharacterCommand& operator= (ConfirmCharacterCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		m_pInitialsComp->ConfirmCharacter();
	}
private:
	InitialsComponent* m_pInitialsComp;
};

#endif // !INITIALSCOMMANDS_H
