#pragma once
#include "Singleton.h"
#include "Controller.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <SDL.h>
#include <glm/glm.hpp>

namespace dae
{
	//enum class ControllerButton;
	enum class KeyState;
	//class Controller;
	class Command;
	class InputManager final : public Singleton<InputManager>
	{
	public:

		bool ProcessInput();
		void AddKeyCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode key, KeyState keyState);
		void AddControllerCommand(std::unique_ptr<Command>&& pCommand, int button, KeyState keyState, uint8_t controllerIndex);
		void AddController();

		bool IsDownThisFrame(SDL_Event& event, SDL_Scancode key) const;
		bool IsUpThisFrame(SDL_Event& event, SDL_Scancode key) const;
		bool IsPressed(SDL_Scancode key)  const;
		
		glm::vec2 GetJoystickValue(bool leftJoystick, uint8_t controllerIndex);
		float GetTriggerValue(bool leftTrigger, uint8_t controllerIndex);

	private:
		std::unordered_map<SDL_Scancode, std::pair<std::unique_ptr<Command>, KeyState>> m_MapCommands;
		std::vector<Controller> m_VecControllers;
	};

}
