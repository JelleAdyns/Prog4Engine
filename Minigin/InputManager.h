#pragma once
#include "Singleton.h"
#include "Controller.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <windows.h>
#include <SDL.h>

namespace dae
{
	
	enum class KeyState;
	class Command;
	class InputManager final : public Singleton<InputManager>
	{
	public:

		bool ProcessInput();
		void AddKeyCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode key, KeyState keyState);
		void AddControllerCommand(std::unique_ptr<Command>&& pCommand, ControllerButton button, KeyState keyState, uint8_t controllerIndex);
		void AddController();

		bool IsDownThisFrame(SDL_Event event, SDL_Scancode key) const;
		bool IsUpThisFrame(SDL_Event event, SDL_Scancode key) const;
		bool IsPressed(SDL_Scancode key)  const;
		
	private:
		std::unordered_map<SDL_Scancode,std::pair<std::unique_ptr<Command>, KeyState>> m_MapCommands{};
		std::vector<Controller> m_VecControllers{};
		
	};

}
