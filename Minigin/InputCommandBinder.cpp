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
		for (auto& controller : m_VecControllers)
		{
			controller->ProcessControllerInput();
		}

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
				}
			}
			ImGui_ImplSDL2_ProcessEvent(&event);
		}

		for (const auto& pair : m_MapKeyCommands)
		{
			auto& commandAndStatePair = pair.second;
			switch (commandAndStatePair.second)
			{
			case KeyState::Pressed:
				if (KeyPressed(pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::NotPressed:
				if (!KeyPressed(pair.first)) commandAndStatePair.first->Execute();
				break;
			}
		}
		return true;
	}



	//Removing
	void InputCommandBinder::RemoveKeyCommand(SDL_Scancode key)
	{
		if (m_MapKeyCommands.contains(key)) m_MapKeyCommands.erase(key);
	}
	void InputCommandBinder::RemoveControllerCommand(int button, uint8_t controllerIndex)
	{
		m_VecControllers.at(controllerIndex)->RemoveCommand(button);
	}
	void InputCommandBinder::PopController()
	{
		if (!m_VecControllers.empty()) m_VecControllers.pop_back();
	}


	//Adding
	void InputCommandBinder::AddKeyCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode key, KeyState keyState)
	{
		if (m_MapKeyCommands.contains(key)) std::cout << "Binding to the requested key already exists. Overwriting now.\n";
		m_MapKeyCommands[key] = std::make_pair(std::move(pCommand), keyState);
	}
	void InputCommandBinder::AddControllerCommand(std::unique_ptr<Command>&& pCommand, int button, KeyState keyState, uint8_t controllerIndex)
	{
		m_VecControllers.at(controllerIndex)->AddCommand(std::move(pCommand), button, keyState);
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
