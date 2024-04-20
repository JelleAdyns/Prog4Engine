#ifndef SETTEXTCOMMAND_H
#define SETTEXTCOMMAND_H

#include <GameObjectCommand.h>
#include <TextComponent.h>


class SetTextCommand final : public dae::GameObjectCommand
{
public:

	SetTextCommand(const std::unique_ptr<dae::GameObject>& pObject, const std::string& newText) :
		GameObjectCommand{ pObject.get() },
		m_NewText{newText}
	{}
	virtual ~SetTextCommand() = default;

	SetTextCommand(const SetTextCommand&) = delete;
	SetTextCommand(SetTextCommand&&) noexcept = delete;
	SetTextCommand& operator= (const SetTextCommand&) = delete;
	SetTextCommand& operator= (SetTextCommand&&) noexcept = delete;

	virtual void Execute() const override
	{
		GetGameObject()->GetComponent<dae::TextComponent>()->SetText(m_NewText);
	}
private:
	const std::string m_NewText;
};


#endif // !SETTEXTCOMMAND_H