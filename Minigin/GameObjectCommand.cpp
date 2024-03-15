#include "GameObjectCommand.h"

namespace dae
{
	GameObjectCommand::GameObjectCommand(GameObject* pGameObject):
		Command{},
		m_GameObject{pGameObject}
	{
	}

	GameObject* GameObjectCommand::GetGameObject() const
	{
		return m_GameObject;
	}

}
