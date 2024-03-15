#pragma once
#include <memory>
#include "Command.h"
#include "GameObject.h"

namespace dae
{
	class GameObjectCommand : public Command
	{
	public:
		virtual ~GameObjectCommand() = default;

		GameObjectCommand(const GameObjectCommand&) = delete;
		GameObjectCommand(GameObjectCommand&&) noexcept = delete;
		GameObjectCommand& operator= (const GameObjectCommand&) = delete;
		GameObjectCommand& operator= (GameObjectCommand&&) noexcept = delete;
	protected:
		GameObjectCommand(GameObject* pGameObject);
		GameObject* GetGameObject() const;
	private:
		GameObject* m_GameObject;
	};
}


