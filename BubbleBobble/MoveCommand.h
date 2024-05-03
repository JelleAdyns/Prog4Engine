#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <GameObjectCommand.h>
#include <GameTime.h>

class MoveCommand final : public dae::GameObjectCommand
{
public:
	MoveCommand(const std::unique_ptr<dae::GameObject>& pObject, int xSpeed, int ySpeed) :
		dae::GameObjectCommand{ pObject.get() },
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
		GetGameObject()->SetLocalPos(currPos + m_DirectionSpeed * dae::GameTime::GetInstance().GetDeltaTime());
	}

private:
	glm::vec2 m_DirectionSpeed;
		
};

#endif // !MOVECOMMAND_H