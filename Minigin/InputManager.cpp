#include "InputManager.h"
#include "GameTime.h"
#include "KeyState.h"
#include <backends/imgui_impl_sdl2.h>
#include <iostream>


bool dae::InputManager::ProcessInput()
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
	while (SDL_PollEvent(&event))
	{
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
			case dae::KeyState::DownThisFrame:
				if (IsDownThisFrame(event, pair.first)) commandAndStatePair.first->Execute();
				break;
			case dae::KeyState::UpThisFrame:
				if (IsUpThisFrame(event, pair.first)) commandAndStatePair.first->Execute();
				break;
			}
		}
	}

	for (const auto& pair : m_MapCommands)
	{
		auto& commandAndStatePair = pair.second;
		switch (commandAndStatePair.second)
		{
		case dae::KeyState::Pressed:
			if (IsPressed(pair.first)) commandAndStatePair.first->Execute();
			break;
		case dae::KeyState::NotPressed:
			if (!IsPressed(pair.first)) commandAndStatePair.first->Execute();
			break;
		}
	}


	for (auto& controller : m_VecControllers)
	{
		controller.ProcessInput();
	}

	for (auto& controller : m_VecControllers)
	{
		controller.HandleInput();
	}

	return true;
}

void dae::InputManager::AddKeyCommand(std::unique_ptr<Command>&& pCommand, SDL_Scancode key, KeyState keyState)
{
	m_MapCommands[key] = std::make_pair(std::move(pCommand), keyState);
}

void dae::InputManager::AddControllerCommand(std::unique_ptr<Command>&& pCommand, ControllerButton button, KeyState keyState, uint8_t controllerIndex)
{
	m_VecControllers.at(controllerIndex).AddCommand(std::move(pCommand), button, keyState);
}

void dae::InputManager::AddController()
{
	if (m_VecControllers.size() < 4)
	{
		m_VecControllers.emplace_back(static_cast<uint8_t>(m_VecControllers.size()) );
	}
	else std::cout << "Max amount of controllers reached.\n";
}


bool dae::InputManager::IsDownThisFrame(SDL_Event event, SDL_Scancode key) const
{
	if (event.type == SDL_KEYDOWN) return event.key.keysym.sym == key;
	else return false;
}

bool dae::InputManager::IsUpThisFrame(SDL_Event event, SDL_Scancode key) const
{
	if (event.type == SDL_KEYUP) return event.key.keysym.sym == key;
	else return false;
}

bool dae::InputManager::IsPressed(SDL_Scancode key) const
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	return pStates[key];
}
