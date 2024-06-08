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

	SelectButtonCommand(const std::unique_ptr<dae::GameObject>& pObject, const std::unique_ptr<dae::GameObject>& pSelectionIcon, float iconOffset, SelectButtonCommand::Direction direction) :
		dae::GameObjectCommand{ pObject.get()},
		m_Direction{direction},
		m_pSelectionIcon{pSelectionIcon.get()},
		m_IconOffset{iconOffset}
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
		m_pSelectionIcon->SetLocalPos(pos.x - m_IconOffset, pos.y);
	}

private:
	Direction m_Direction;
	dae::GameObject* m_pSelectionIcon;
	const float m_IconOffset;
};


#endif // !SELECTBUTTONCOMMAND_H