#ifndef INPUTCOMMANDBINDER_H
#define	INPUTCOMMANDBINDER_H

#include "Singleton.h"
#include <unordered_map>
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

		bool ProcessInput();

		void AddKeyCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode key, KeyState keyState);
		void AddControllerCommand(std::unique_ptr<Command>&& pCommand, ControllerButton button, KeyState keyState, uint8_t controllerIndex);
		void AddController();

		void RemoveAllCommands();
		void RemoveKeyCommand(SDL_Scancode key);
		void RemoveControllerCommand(ControllerButton button, uint8_t controllerIndex);
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

		std::unordered_map<SDL_Scancode, std::pair<std::unique_ptr<Command>, KeyState>> m_MapKeyCommands;
		
		std::vector<std::unique_ptr<Controller>> m_VecControllers;
	};

}
#endif // !INPUTCOMMANDBINDER_H