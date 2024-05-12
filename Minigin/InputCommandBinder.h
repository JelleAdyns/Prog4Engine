#ifndef INPUTCOMMANDBINDER_H
#define	INPUTCOMMANDBINDER_H

#include "Singleton.h"
#include <map>
#include <vector>
#include <memory>
#include <SDL.h>
#include <glm/glm.hpp>
#include "Controller.h"

namespace dae
{
	enum class KeyState;
	class Command;
	class InputCommandBinder final : public Singleton<InputCommandBinder>
	{
	public:
		virtual ~InputCommandBinder() = default;

		InputCommandBinder(const InputCommandBinder&) = delete;
		InputCommandBinder(InputCommandBinder&&) noexcept = delete;
		InputCommandBinder& operator= (const InputCommandBinder&) = delete;
		InputCommandBinder& operator= (InputCommandBinder&&) noexcept = delete;

		bool IsKeyboardActive() const;

		bool ProcessInput();

		void AddKeyCommand(const std::shared_ptr<Command>& pCommand, SDL_Scancode key, KeyState keyState);
		void AddControllerCommand(const std::shared_ptr<Command>& pCommand, ControllerButton button, KeyState keyState, uint8_t controllerIndex);
		void AddCommand_ChangingToController(std::unique_ptr<Command>&& pCommand);
		void AddCommand_ChangingToKeyboard(std::unique_ptr<Command>&& pCommand);
		void AddController();

		void RemoveAllCommands();
		void RemoveKeyCommand(SDL_Scancode key, KeyState keyState);
		void RemoveControllerCommand(ControllerButton button, KeyState keyState, uint8_t controllerIndex);
		void PopController();
		void PopAllControllers();

		bool KeyDownThisFrame(SDL_Event& event, SDL_Scancode key) const;
		bool KeyUpThisFrame(SDL_Event& event, SDL_Scancode key) const;
		bool KeyPressed(SDL_Scancode key) const;

		bool ButtonDownThisFrame(ControllerButton button, uint8_t controllerIndex) const;
		bool ButtonUpThisFrame(ControllerButton button, uint8_t controllerIndex) const;
		bool ButtonPressed(ControllerButton button, uint8_t controllerIndex)  const;
		
		glm::vec2 GetJoystickValue(bool leftJoystick, uint8_t controllerIndex);
		float GetTriggerValue(bool leftTrigger, uint8_t controllerIndex);

	private:
		friend class Singleton<InputCommandBinder>;
		InputCommandBinder() = default;

		bool m_IsKeyboardActive{ true };

		struct KeyBoardState
		{
			SDL_Scancode key;
			KeyState keyState;

			bool operator<(const KeyBoardState& other) const
			{
				if (key != other.key)
					return other.key < key;
				return other.keyState < keyState;
			}

			bool operator==(const KeyBoardState& other) const 
			{
				return other.key == key && other.keyState == keyState;
			}
		};

		std::map<KeyBoardState, std::shared_ptr<Command>> m_MapKeyCommands;
		
		std::vector<std::unique_ptr<Command>> m_pVecCommandsChangingToController;
		std::vector<std::unique_ptr<Command>> m_pVecCommandsChangingToKeyboard;

		std::vector<std::unique_ptr<Controller>> m_pVecControllers;
	};

}
#endif // !INPUTCOMMANDBINDER_H