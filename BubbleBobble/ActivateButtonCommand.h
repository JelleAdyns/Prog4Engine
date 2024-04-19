#ifndef ACTIVATEBUTTONCOMMAND_H
#define ACTIVATEBUTTONCOMMAND_H

#include <GameObjectCommand.h>
#include "ButtonHandlerComponent.h"


class ActivateButtonCommand final : public dae::GameObjectCommand
{
public:

	ActivateButtonCommand(const std::unique_ptr<dae::GameObject>& pObject) :
		GameObjectCommand{ pObject.get() }
	{}
	virtual ~ActivateButtonCommand() = default;

	ActivateButtonCommand(const ActivateButtonCommand&) = delete;
	ActivateButtonCommand(ActivateButtonCommand&&) noexcept = delete;
	ActivateButtonCommand& operator= (const ActivateButtonCommand&) = delete;
	ActivateButtonCommand& operator= (ActivateButtonCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		GetGameObject()->GetComponent<ButtonHandlerComponent>()->Activate();
	}

};


#endif // !ACTIVATEBUTTONCOMMAND_H