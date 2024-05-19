#include "InputCommandBinder.h"
#include "InputCommandBinder.h"
#include "InputCommandBinder.h"
#include "GameTime.h"
#include "KeyState.h"
#include <backends/imgui_impl_sdl2.h>
#include <iostream>
#include <algorithm>

#include <windows.h>
#include <SDL.h>



namespace dae
{
	bool InputCommandBinder::IsKeyboardActive() const
	{
		return m_IsKeyboardActive;
	}
	bool InputCommandBinder::ProcessInput()
	{
		for (auto& pController : m_pVecControllers)
		{
			if (pController->IsAnyButtonPressed())
			{
				m_IsKeyboardActive = false;
				for (auto& pCommand : m_pVecCommandsChangingToController) pCommand->Execute();
			}
		}

		SDL_Event event{};
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN) 
			{
				m_IsKeyboardActive = true;
				for (auto& pCommand : m_pVecCommandsChangingToKeyboard) pCommand->Execute();
			}
			if (event.type == SDL_QUIT) return false;

			if(m_IsKeyboardActive)
			{
				/*if (event.key.keysym.sym == SDLK_SPACE)
				{
					GameTime::GetInstance().ToggleUseFPSGoal();
				}*/

				for (const auto& pair : m_MapKeyCommands)
				{
					auto& pCommand = pair.second;
					switch (pair.first.keyState)
					{
					case KeyState::DownThisFrame:
						if (KeyDownThisFrame(event, pair.first.key)) pCommand->Execute();
						break;
					case KeyState::UpThisFrame:
						if (KeyUpThisFrame(event, pair.first.key)) pCommand->Execute();
						break;
					}
				}
			}

			ImGui_ImplSDL2_ProcessEvent(&event);
		}

		if (m_IsKeyboardActive)
		{
			for (const auto& pair : m_MapKeyCommands)
			{
				auto& pCommand = pair.second;
				switch (pair.first.keyState)
				{
				case KeyState::Pressed:
					if (KeyPressed(pair.first.key)) pCommand->Execute();
					break;
				case KeyState::NotPressed:
					if (!KeyPressed(pair.first.key)) pCommand->Execute();
					break;
				}
			}
		}

		if (!m_IsKeyboardActive)
		{
			for (auto& controller : m_pVecControllers)
			{
				controller->ProcessControllerInput();
			}
		}

		return true;
		
	}

	
	//Removing
	void InputCommandBinder::RemoveAllCommands()
	{
		m_MapKeyCommands.clear();
		m_pVecCommandsChangingToController.clear();
		m_pVecCommandsChangingToKeyboard.clear();
		for (auto& controller : m_pVecControllers)
		{
			controller->RemoveAllCommands();
		}
	}
	void InputCommandBinder::RemoveKeyCommand(SDL_Scancode key, KeyState keyState)
	{
		KeyBoardState state{};
		state.key = key;
		state.keyState = keyState;
		if (m_MapKeyCommands.contains(state)) m_MapKeyCommands.erase(state);
	}
	void InputCommandBinder::RemoveControllerCommand(ControllerButton button, KeyState keyState, uint8_t controllerIndex)
	{
		m_pVecControllers.at(controllerIndex)->RemoveCommand(button, keyState);
	}
	void InputCommandBinder::PopController()
	{
		if (!m_pVecControllers.empty()) m_pVecControllers.pop_back();
	}
	void InputCommandBinder::PopAllControllers()
	{
		m_pVecControllers.clear();
	}


	//Adding
	void InputCommandBinder::AddKeyCommand(const std::shared_ptr<Command>& pCommand, SDL_Scancode key, KeyState keyState)
	{
		KeyBoardState state{};
		state.key = key;
		state.keyState = keyState;
#ifndef NDEBUG
		if (m_MapKeyCommands.contains(state)) std::cout << "Binding to the requested key ("<<key<<") already exists.Overwriting now.\n";
#endif // !NDEBUG
		m_MapKeyCommands[state] = pCommand;
	}
	void InputCommandBinder::AddControllerCommand(const std::shared_ptr<Command>& pCommand, ControllerButton button, KeyState keyState, uint8_t controllerIndex)
	{
		m_pVecControllers.at(controllerIndex)->AddCommand(pCommand, button, keyState);
	}
	void InputCommandBinder::AddCommand_ChangingToController(std::unique_ptr<Command>&& pCommand)
	{
		m_pVecCommandsChangingToController.emplace_back(std::move(pCommand));
	}
	void InputCommandBinder::AddCommand_ChangingToKeyboard(std::unique_ptr<Command>&& pCommand)
	{
		m_pVecCommandsChangingToKeyboard.emplace_back(std::move(pCommand));
	}
	void InputCommandBinder::AddController()
	{
		if (m_pVecControllers.size() < 4)
		{
			m_pVecControllers.emplace_back(std::make_unique<Controller>( static_cast<uint8_t>(m_pVecControllers.size()) ));
		}
#ifndef NDEBUG
		else std::cout << "Max amount of controllers already reached.\n";
#endif // !NDEBUG
		
	}


	//Key Checks
	bool InputCommandBinder::KeyDownThisFrame(SDL_Event& event, SDL_Scancode key) const
	{
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) return event.key.keysym.scancode == key;
		else return false;
	}
	bool InputCommandBinder::KeyUpThisFrame(SDL_Event& event, SDL_Scancode key) const
	{
		if (event.type == SDL_KEYUP) return event.key.keysym.scancode == key;
		else return false;
	}
	bool InputCommandBinder::KeyPressed(SDL_Scancode key) const
	{
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);

		return pStates[key];
	}


	//Controller button checks
	bool InputCommandBinder::ButtonDownThisFrame(ControllerButton button, uint8_t controllerIndex) const
	{
		return m_pVecControllers.at(controllerIndex)->IsDownThisFrame(button);
	}
	bool InputCommandBinder::ButtonUpThisFrame(ControllerButton button, uint8_t controllerIndex) const
	{
		return m_pVecControllers.at(controllerIndex)->IsUpThisFrame(button);
	}
	bool InputCommandBinder::ButtonPressed(ControllerButton button, uint8_t controllerIndex) const
	{
		return m_pVecControllers.at(controllerIndex)->IsPressed(button);
	}
	void InputCommandBinder::VibrateController(int strengthPercentage, uint8_t controllerIndex)
	{
		if (controllerIndex < m_pVecControllers.size())
		{
			m_pVecControllers.at(controllerIndex)->Vibrate(strengthPercentage);
		}
#ifndef NDEBUG
		else std::cout << "Trying to vibrate controller, but controller for controllerIndex ("<< controllerIndex << ") not found.\n";
#endif // !NDEBUG
	}
	glm::vec2 InputCommandBinder::GetJoystickValue(bool leftJoystick, uint8_t controllerIndex)
	{
		return m_pVecControllers.at(controllerIndex)->GetJoystickValue(leftJoystick);
	}
	float InputCommandBinder::GetTriggerValue(bool leftTrigger, uint8_t controllerIndex)
	{
		return m_pVecControllers.at(controllerIndex)->GetTriggerValue(leftTrigger);
	}



}
