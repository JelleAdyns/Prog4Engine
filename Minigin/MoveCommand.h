#pragma once
#include "GameObjectCommand.h"

namespace dae
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(const std::unique_ptr<GameObject>& pObject, int xSpeed, int ySpeed) :
			GameObjectCommand{ pObject.get() },
			m_MoveSpeed{xSpeed,ySpeed}
		{}
		virtual ~MoveCommand() = default;

		MoveCommand(const MoveCommand&) = delete;
		MoveCommand(MoveCommand&&) noexcept = delete;
		MoveCommand& operator= (const MoveCommand&) = delete;
		MoveCommand& operator= (MoveCommand&&) noexcept = delete;
		virtual void Execute() const override
		{
			glm::vec2 world = GetGameObject()->GetWorldPosition();
			GetGameObject()->SetLocalPos(world + m_MoveSpeed);
		}
	private:
		glm::vec2 m_MoveSpeed;
		
	};
}
