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

	bool InputCommandBinder::ProcessInput()
	{
		HandleControllerInput();
		return HandleInput();
	}

	bool InputCommandBinder::HandleInput()
	{
		SDL_Event event{};
		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
			{
				return false;
			}
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				GameTime::GetInstance().ToggleUseFPSGoal();
			}

			for (const auto& pair : m_MapKeyCommands)
			{
				auto& commandAndStatePair = pair.second;
				switch (commandAndStatePair.second)
				{
				case KeyState::DownThisFrame:
					if (KeyDownThisFrame(event, pair.first)) commandAndStatePair.first->Execute();
					break;
				case KeyState::UpThisFrame:
					if (KeyUpThisFrame(event, pair.first)) commandAndStatePair.first->Execute();
					break;
				case KeyState::Pressed:
					if (KeyPressed(pair.first)) commandAndStatePair.first->Execute();
					break;
				case KeyState::NotPressed:
					if (!KeyPressed(pair.first)) commandAndStatePair.first->Execute();
					break;
				}
			}
			ImGui_ImplSDL2_ProcessEvent(&event);
		}
		return true;
	}

	void InputCommandBinder::HandleControllerInput()
	{

		for (uint8_t i = 0; i < m_VecControllers.size(); ++i)
		{
			m_VecControllers.at(i)->ProcessControllerInput();

		}
		for (const auto& controllerInfo : m_VecControllerCommands)
		{

			switch (controllerInfo.keyState)
			{
			case KeyState::DownThisFrame:
				if (ButtonDownThisFrame(controllerInfo.button, controllerInfo.controllerIndex)) controllerInfo.command->Execute();
				break;
			case KeyState::UpThisFrame:
				if (ButtonUpThisFrame(controllerInfo.button, controllerInfo.controllerIndex)) controllerInfo.command->Execute();
				break;
			case KeyState::Pressed:
				if (ButtonPressed(controllerInfo.button, controllerInfo.controllerIndex)) controllerInfo.command->Execute();
				break;
			case KeyState::NotPressed:
				if (!ButtonPressed(controllerInfo.button, controllerInfo.controllerIndex)) controllerInfo.command->Execute();
				break;
			}

		}
	}


	//Removing
	void InputCommandBinder::RemoveKeyCommand(SDL_Scancode key)
	{
		if (m_MapKeyCommands.contains(key)) m_MapKeyCommands.erase(key);
	}
	void InputCommandBinder::RemoveControllerCommand(int button, uint8_t controllerIndex)
	{
		auto pos = std::find_if(m_VecControllerCommands.cbegin(), m_VecControllerCommands.cend(), [&](const ControllerCommandInfo& info)
			{
				return info.button == button && info.controllerIndex == controllerIndex;
			});

		if(pos == m_VecControllerCommands.cend()) std::cout << "There was no command bound to the controller and button while trying to remove the binding, you're good to go.\n";
		else m_VecControllerCommands.erase(pos);
	
	}
	void InputCommandBinder::PopController()
	{
		if (!m_VecControllers.empty()) m_VecControllers.pop_back();

		uint8_t controllerIndex = static_cast<uint8_t>(m_VecControllers.size());

		m_VecControllerCommands.erase(
			std::remove_if(m_VecControllerCommands.begin(), m_VecControllerCommands.end(), [&](const ControllerCommandInfo& info)
				{
					return info.controllerIndex == controllerIndex;
				}
			),
			m_VecControllerCommands.end());

	}


	//Adding
	void InputCommandBinder::AddKeyCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode key, KeyState keyState)
	{
		if (m_MapKeyCommands.contains(key)) std::cout << "Binding to the requested key already exists. Overwriting now.\n";
		m_MapKeyCommands[key] = std::make_pair(std::move(pCommand), keyState);
	}
	void InputCommandBinder::AddControllerCommand(std::unique_ptr<Command>&& pCommand, int button, KeyState keyState, uint8_t controllerIndex)
	{
		
		auto pos = std::find_if(m_VecControllerCommands.cbegin(), m_VecControllerCommands.cend(), [&](const ControllerCommandInfo& info)
			{
				return info.button == button && info.controllerIndex == controllerIndex;
			});
		if (pos != m_VecControllerCommands.cend()) throw std::runtime_error("The requested button and controller are already bound! Remove the binding before adding the new command.");

		ControllerCommandInfo info;
		info.command = std::move(pCommand);
		info.keyState = keyState;
		info.button = button;
		info.controllerIndex = controllerIndex;

		m_VecControllerCommands.push_back(std::move(info));
	}
	void InputCommandBinder::AddController()
	{
		if (m_VecControllers.size() < 4)
		{
			m_VecControllers.emplace_back(std::make_unique<Controller>( static_cast<uint8_t>(m_VecControllers.size()) ));
		}
		else std::cout << "Max amount of controllers already reached.\n";
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
	bool InputCommandBinder::ButtonDownThisFrame(int button, uint8_t controllerIndex) const
	{
		return m_VecControllers.at(controllerIndex)->IsDownThisFrame(button);
	}
	bool InputCommandBinder::ButtonUpThisFrame(int button, uint8_t controllerIndex) const
	{
		return m_VecControllers.at(controllerIndex)->IsUpThisFrame(button);
	}
	bool InputCommandBinder::ButtonPressed(int button, uint8_t controllerIndex) const
	{
		return m_VecControllers.at(controllerIndex)->IsPressed(button);
	}
	glm::vec2 InputCommandBinder::GetJoystickValue(bool leftJoystick, uint8_t controllerIndex)
	{
		return m_VecControllers.at(controllerIndex)->GetJoystickValue(leftJoystick);
	}
	float InputCommandBinder::GetTriggerValue(bool leftTrigger, uint8_t controllerIndex)
	{
		return m_VecControllers.at(controllerIndex)->GetTriggerValue(leftTrigger);
	}



}
