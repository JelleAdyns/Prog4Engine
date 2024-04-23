#ifndef KILLCOMMAND_H
#define KILLCOMMAND_H

#include <GameObjectCommand.h>
#include "LivesComponent.h"

class KillCommand final : public dae::GameObjectCommand
{
public:
	KillCommand(const std::unique_ptr<dae::GameObject>& pObject) :
		dae::GameObjectCommand{ pObject.get() }
	{}
	virtual ~KillCommand() = default;

	KillCommand(const KillCommand&) = delete;
	KillCommand(KillCommand&&) noexcept = delete;
	KillCommand& operator= (const KillCommand&) = delete;
	KillCommand& operator= (KillCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		GetGameObject()->GetComponent<LivesComponent>()->TakeLife();
	}

};
#endif // !KILLCOMMAND_H