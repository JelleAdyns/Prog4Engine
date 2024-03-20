#ifndef COMMAND_H
#define COMMAND_H
namespace dae
{
	class Command
	{
	public:
		Command() = default;
		virtual ~Command() = default;

		Command(const Command&) = delete;
		Command(Command&&) noexcept = delete;
		Command& operator= (const Command&) = delete;
		Command& operator= (Command&&) noexcept = delete;

		virtual void Execute() const = 0;
	};

}

#endif // !COMMAND_H