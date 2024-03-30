#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H

#include <memory>
#include "Command.h"

namespace dae
{
	class EditorCommand : public Command
	{
	public:
		EditorCommand() :
			Command{}
		{}
		virtual ~EditorCommand() = default;

		EditorCommand(const EditorCommand&) = delete;
		EditorCommand(EditorCommand&&) noexcept = delete;
		EditorCommand& operator= (const EditorCommand&) = delete;
		EditorCommand& operator= (EditorCommand&&) noexcept = delete;

		virtual void Undo() = 0;
	};
}
#endif // !EDITORCOMMAND_H