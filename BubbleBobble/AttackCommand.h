#ifndef ATTACKCOMMAND_H
#define ATTACKCOMMAND_H

#include <GameObjectCommand.h>
#include "PlayerComponent.h"
#include "EnemyComponent.h"

class AttackCommand final : public dae::GameObjectCommand
{
public:
	AttackCommand(const std::unique_ptr<dae::GameObject>& pObject) :
		AttackCommand{ pObject.get() }
	{}
	AttackCommand(dae::GameObject* pObject) :
		dae::GameObjectCommand{ pObject },
		m_pEnemyComp{nullptr},
		m_pPlayerComp{nullptr}
	{
		if(GetGameObject()->HasComponent<EnemyComponent>())
			m_pEnemyComp = pObject->GetComponent<EnemyComponent>(); 
		else m_pPlayerComp = pObject->GetComponent<PlayerComponent>(); 
	}
	virtual ~AttackCommand() = default;

	AttackCommand(const AttackCommand&) = delete;
	AttackCommand(AttackCommand&&) noexcept = delete;
	AttackCommand& operator= (const AttackCommand&) = delete;
	AttackCommand& operator= (AttackCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		if (m_pPlayerComp) m_pPlayerComp->Shoot();
		else m_pEnemyComp->Attack();
	}
private:
	PlayerComponent* m_pPlayerComp;
	EnemyComponent* m_pEnemyComp;
};
#endif // !ATTACKCOMMAND_H