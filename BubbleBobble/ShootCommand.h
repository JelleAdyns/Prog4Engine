#ifndef SHOOTCOMMAND_H
#define SHOOTCOMMAND_H

#include <GameObjectCommand.h>
#include "PlayerComponent.h"

class ShootCommand final : public dae::GameObjectCommand
{
public:
	ShootCommand(const std::unique_ptr<dae::GameObject>& pObject) :
		ShootCommand{ pObject.get() }
	{

	}
	ShootCommand(dae::GameObject* pObject) :
		dae::GameObjectCommand{ pObject },
		m_pPlayerComp{pObject->GetComponent<PlayerComponent>()}
	{}
	virtual ~ShootCommand() = default;

	ShootCommand(const ShootCommand&) = delete;
	ShootCommand(ShootCommand&&) noexcept = delete;
	ShootCommand& operator= (const ShootCommand&) = delete;
	ShootCommand& operator= (ShootCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		m_pPlayerComp->Shoot();
	}
private:
	PlayerComponent* m_pPlayerComp;
};
#endif // !SHOOTCOMMAND_H