#pragma once
#include "GameObjectCommand.h"
#include "GameTime.h"

namespace dae
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(const std::unique_ptr<GameObject>& pObject, int xSpeed, int ySpeed) :
			GameObjectCommand{ pObject.get() },
			m_DirectionSpeed{xSpeed,ySpeed}
		{}
		virtual ~MoveCommand() = default;

		MoveCommand(const MoveCommand&) = delete;
		MoveCommand(MoveCommand&&) noexcept = delete;
		MoveCommand& operator= (const MoveCommand&) = delete;
		MoveCommand& operator= (MoveCommand&&) noexcept = delete;
		virtual void Execute() const override
		{
			glm::vec2 currPos = GetGameObject()->GetLocalPosition();
			GetGameObject()->SetLocalPos(currPos + m_DirectionSpeed * GameTime::GetInstance().GetDeltaTime());
		}
		void SetSpeed(int xSpeed, int ySpeed)
		{
			m_DirectionSpeed = glm::vec2{ xSpeed, ySpeed };
		}
	private:
		glm::vec2 m_DirectionSpeed;
		
	};
}
