#include "InputManager.h"
#include "GameTime.h"
#include "KeyState.h"
#include <backends/imgui_impl_sdl2.h>
#include <iostream>
//#include "Controller.h"
#include <windows.h>

//#include "Controller.h"

namespace dae
{

	bool InputManager::ProcessInput()
	{
		//if (event.key.keysym.scancode == SDL_Scancode_1)
		//if (event.type == SDL_KEYUP) {
		//	if (event.key.keysym.sym == SDLK_SPACE)
		//	{
		//		GameTime::GetInstance().ToggleUseFPSGoal();
		//	}
		//}
		//if (event.type == SDL_MOUSEBUTTONDOWN) {
		//}

		SDL_Event event;
		SDL_PollEvent(&event);

		ImGui_ImplSDL2_ProcessEvent(&event);

		if (event.type == SDL_QUIT) return false;
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			GameTime::GetInstance().ToggleUseFPSGoal();
		}

		for (const auto& pair : m_MapCommands)
		{
			auto& commandAndStatePair = pair.second;
			switch (commandAndStatePair.second)
			{
			case KeyState::DownThisFrame:
				if (IsDownThisFrame(event, pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::UpThisFrame:
				if (IsUpThisFrame(event, pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::Pressed:
				if (IsPressed(pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::NotPressed:
				if (!IsPressed(pair.first)) commandAndStatePair.first->Execute();
				break;
			}
		}

		for (auto& controller : m_VecControllers)
		{
			controller.ProcessInput();
		}

		return true;
	}

	void InputManager::AddKeyCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode key, KeyState keyState)
	{
		m_MapCommands[key] = std::make_pair(std::move(pCommand), keyState);
	}

	void InputManager::AddControllerCommand(std::unique_ptr<Command>&& pCommand, int button, KeyState keyState, uint8_t controllerIndex)
	{
		m_VecControllers.at(controllerIndex).AddCommand(std::move(pCommand), button, keyState);
	}

	void InputManager::AddController()
	{
		if (m_VecControllers.size() < 4)
		{
			m_VecControllers.emplace_back(static_cast<uint8_t>(m_VecControllers.size()));
		}
		else std::cout << "Max amount of controllers reached.\n";
	}

	bool InputManager::IsDownThisFrame(SDL_Event& event, SDL_Scancode key) const
	{
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) return event.key.keysym.scancode == key;
		else return false;
	}

	bool InputManager::IsUpThisFrame(SDL_Event& event, SDL_Scancode key) const
	{
		if (event.type == SDL_KEYUP) return event.key.keysym.scancode == key;
		else return false;
	}

	bool InputManager::IsPressed(SDL_Scancode key) const
	{
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);

		return pStates[key];
	}

	glm::vec2 InputManager::GetJoystickValue(bool leftJoystick, uint8_t controllerIndex)
	{
		return m_VecControllers.at(controllerIndex).GetJoystickValue(leftJoystick);
	}

	float InputManager::GetTriggerValue(bool leftTrigger, uint8_t controllerIndex)
	{
		return m_VecControllers.at(controllerIndex).GetTriggerValue(leftTrigger);
	}

}

