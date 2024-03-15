#include "Controller.h"
#include "InputManager.h"
#include "KeyState.h"
namespace dae
{

	bool Controller::ProcessInput()
	{
		m_PreviousState = m_CurrentState;
		m_CurrentState = XINPUT_STATE{};
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

		HandleInput();

		return true;
	}

	void Controller::HandleInput() const
	{

		for (const auto& pair : m_MapCommands)
		{
			auto& commandAndStatePair = pair.second;
			switch (commandAndStatePair.second)
			{
			case KeyState::DownThisFrame:
				if (IsDownThisFrame(pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::UpThisFrame:
				if (IsUpThisFrame(pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::Pressed:
				if (IsPressed(pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::NotPressed:
				if (!IsPressed(pair.first)) commandAndStatePair.first->Execute();
				break;
			}
		}
		
	}

	bool Controller::IsDownThisFrame(ControllerButton button) const
	{
		return m_ButtonsPressedThisFrame & static_cast<int>(button);
	}

	bool Controller::IsUpThisFrame(ControllerButton button) const
	{
		return m_ButtonsReleasedThisFrame & static_cast<int>(button);
	}
	bool Controller::IsPressed(ControllerButton button) const
	{
		return m_CurrentState.Gamepad.wButtons & static_cast<int>(button);
	}
	void Controller::AddCommand(std::unique_ptr<Command>&& pCommand, ControllerButton button, KeyState keyState)
	{
		
		m_MapCommands[button] = std::make_pair(std::move(pCommand), keyState);
		
	}
}