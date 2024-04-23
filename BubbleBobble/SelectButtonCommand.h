#ifndef SELECTBUTTONCOMMAND_H
#define SELECTBUTTONCOMMAND_H

#include <GameObjectCommand.h>
#include "ButtonHandlerComponent.h"


class SelectButtonCommand final : public dae::GameObjectCommand
{
public:
	enum class Direction
	{
		Previous,
		Next
	};

	SelectButtonCommand(const std::unique_ptr<dae::GameObject>& pObject, const std::unique_ptr<dae::GameObject>& pSelectionIcon, SelectButtonCommand::Direction direction) :
		dae::GameObjectCommand{ pObject.get()},
		m_Direction{direction},
		m_pSelectionIcon{pSelectionIcon.get()}
	{}
	virtual ~SelectButtonCommand() = default;

	SelectButtonCommand(const SelectButtonCommand&) = delete;
	SelectButtonCommand(SelectButtonCommand&&) noexcept = delete;
	SelectButtonCommand& operator= (const SelectButtonCommand&) = delete;
	SelectButtonCommand& operator= (SelectButtonCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		const auto& pButtonHandler = GetGameObject()->GetComponent<ButtonHandlerComponent>();

		switch (m_Direction)
		{
		case Direction::Previous:
			pButtonHandler->SelectPrevious();
			break;
		case Direction::Next:
			pButtonHandler->SelectNext();
			break;
		}
		auto pos = pButtonHandler->GetSelectedButton()->GetPos();
		m_pSelectionIcon->SetLocalPos(pos.x - 20, pos.y);
	}

private:
	Direction m_Direction;
	dae::GameObject* m_pSelectionIcon;
};


#endif // !SELECTBUTTONCOMMAND_H