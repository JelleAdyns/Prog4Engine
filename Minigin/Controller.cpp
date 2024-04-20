#include "Controller.h"
#include "KeyState.h"
#include <unordered_map>
#include <iostream>

namespace dae
{

	//Implementation
	class Controller::ControllerImpl
	{
	public:
		ControllerImpl(uint8_t controllerIndex):
			m_ControllerIndex{controllerIndex},
			m_PreviousState{},
			m_CurrentState{},
			m_ButtonsPressedThisFrame{},
			m_ButtonsReleasedThisFrame{}
		{
	
		}

		~ControllerImpl() = default;
		ControllerImpl(const ControllerImpl&) = delete;
		ControllerImpl(ControllerImpl&&) noexcept = default;
		ControllerImpl& operator= (const ControllerImpl&) = delete;
		ControllerImpl& operator= (ControllerImpl&&) noexcept = delete;


		bool IsAnyButtonPressedImpl();
		void ProcessInputImpl();
		bool IsDownThisFrameImpl(ControllerButton button) const;
		bool IsUpThisFrameImpl(ControllerButton button)  const;
		bool IsPressedImpl(ControllerButton button)  const;

		void AddCommandImpl(const std::shared_ptr<Command>& pCommand, ControllerButton button, KeyState keyState);
		void RemoveCommandImpl(ControllerButton button);
		void RemoveAllCommandsImpl();

		glm::vec2 GetJoystickValueImpl(bool leftJoystick);
		float GetTriggerValueImpl(bool leftJoystick);

	private:
		uint8_t m_ControllerIndex{};
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedThisFrame{};

		std::unordered_map<ControllerButton, std::pair<std::shared_ptr<Command>, KeyState>> m_MapCommands{};

		static float m_MaxJoystickValue;
		static float m_JoystickDeadZonePercentage;
		static float m_MaxTriggerValue;
		static float m_TriggerDeadZonePercentage;

		void HandleInputImpl() const;
	};


	float Controller::ControllerImpl::m_MaxJoystickValue = static_cast<float>(SHRT_MAX);
	float Controller::ControllerImpl::m_JoystickDeadZonePercentage = 30.f;
	float Controller::ControllerImpl::m_MaxTriggerValue = static_cast<float>(_UI8_MAX);
	float Controller::ControllerImpl::m_TriggerDeadZonePercentage = 50.f;

	void Controller::ControllerImpl::HandleInputImpl() const
	{
		for (const auto& pair : m_MapCommands)
		{
			auto& commandAndStatePair = pair.second;
			switch (commandAndStatePair.second)
			{
			case KeyState::DownThisFrame:
				if (IsDownThisFrameImpl(pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::UpThisFrame:
				if (IsUpThisFrameImpl(pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::Pressed:
				if (IsPressedImpl(pair.first)) commandAndStatePair.first->Execute();
				break;
			case KeyState::NotPressed:
				if (!IsPressedImpl(pair.first)) commandAndStatePair.first->Execute();
				break;
			}
		}
	}
	bool Controller::ControllerImpl::IsAnyButtonPressedImpl()
	{
		XINPUT_STATE previousState = m_CurrentState;
		XINPUT_STATE currentState = XINPUT_STATE{};
		XInputGetState(m_ControllerIndex, &currentState);

		//buttons
		auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;

		//Joysticks
		float xL, yL, xR, yR;
		xL = currentState.Gamepad.sThumbLX / m_MaxJoystickValue;
		yL = currentState.Gamepad.sThumbLY / m_MaxJoystickValue;

		xR = currentState.Gamepad.sThumbRX / m_MaxJoystickValue;
		yR = currentState.Gamepad.sThumbRY / m_MaxJoystickValue;
	
		if (std::abs(xL) < m_JoystickDeadZonePercentage / 100.f) xL = 0;
		if (std::abs(yL) < m_JoystickDeadZonePercentage / 100.f) yL = 0;
		if (std::abs(xR) < m_JoystickDeadZonePercentage / 100.f) xR = 0;
		if (std::abs(yR) < m_JoystickDeadZonePercentage / 100.f) yR = 0;

		//Triggers
		float valueL, valueR;
		valueL = currentState.Gamepad.bLeftTrigger / m_MaxTriggerValue;
		valueR = currentState.Gamepad.bRightTrigger / m_MaxTriggerValue;

		if (valueL < m_TriggerDeadZonePercentage / 100.f) valueL = 0;
		if (valueR < m_TriggerDeadZonePercentage / 100.f) valueR = 0;

		return (buttonChanges ||
			xL >= 0.2f ||
			yL >= 0.2f ||
			xR >= 0.2f ||
			yR >= 0.2f ||
			valueL >= 0.2f ||
			valueR >= 0.2f);
	}
	void Controller::ControllerImpl::ProcessInputImpl()
	{
		
		m_PreviousState = m_CurrentState;
		m_CurrentState = XINPUT_STATE{};
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		HandleInputImpl();

		//XINPUT_VIBRATION vibration;
		//ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		//vibration.wLeftMotorSpeed = 32000; // use any value between 0-65535 here
		//vibration.wRightMotorSpeed = 16000; // use any value between 0-65535 here
		//XInputSetState(m_ControllerIndex, &vibration);
	}

	bool Controller::ControllerImpl::IsDownThisFrameImpl(ControllerButton button) const
	{
		return m_ButtonsPressedThisFrame & static_cast<int>(button);
	}
	bool Controller::ControllerImpl::IsUpThisFrameImpl(ControllerButton button) const
	{
		return m_ButtonsReleasedThisFrame & static_cast<int>(button);
	}
	bool Controller::ControllerImpl::IsPressedImpl(ControllerButton button) const
	{
		return m_CurrentState.Gamepad.wButtons & static_cast<int>(button);
	}

	void Controller::ControllerImpl::AddCommandImpl(const std::shared_ptr<Command>& pCommand, ControllerButton button, KeyState keyState)
	{
#ifndef NDEBUG
		if (m_MapCommands.contains(button)) std::cout << "Binding to the requested button already exists. Overwriting now.\n";
#endif // !NDEBUG
		m_MapCommands[button] = std::make_pair(pCommand, keyState);
	}

	void Controller::ControllerImpl::RemoveCommandImpl(ControllerButton button)
	{
		if (m_MapCommands.contains(button)) m_MapCommands.erase(button);
	}

	void Controller::ControllerImpl::RemoveAllCommandsImpl()
	{
		m_MapCommands.clear();
	}


	glm::vec2 Controller::ControllerImpl::GetJoystickValueImpl(bool leftJoystick)
	{
		float x, y;
		if (leftJoystick)
		{
			x = m_CurrentState.Gamepad.sThumbLX / m_MaxJoystickValue;
			y = m_CurrentState.Gamepad.sThumbLY / m_MaxJoystickValue;
		}
		else
		{
			x = m_CurrentState.Gamepad.sThumbRX / m_MaxJoystickValue;
			y = m_CurrentState.Gamepad.sThumbRY / m_MaxJoystickValue;
		}

		if (std::abs(x) < m_JoystickDeadZonePercentage / 100.f) x = 0;
		if (std::abs(y) < m_JoystickDeadZonePercentage / 100.f) y = 0;

		return glm::vec2{ x, -y };
	}

	float Controller::ControllerImpl::GetTriggerValueImpl(bool leftJoystick)
	{
		float value;
		if (leftJoystick) value = m_CurrentState.Gamepad.bLeftTrigger / m_MaxTriggerValue;
		else value = m_CurrentState.Gamepad.bRightTrigger / m_MaxTriggerValue;

		if (value < m_TriggerDeadZonePercentage / 100.f) value = 0;

		return value;
	}

	//Controller
	Controller::Controller(uint8_t controllerIndex) : 
		m_pImpl{ new ControllerImpl{controllerIndex} }
	{}

	Controller::~Controller()
	{
		delete m_pImpl;
	}

	bool Controller::IsAnyButtonPressed()
	{
		return m_pImpl->IsAnyButtonPressedImpl();
	}

	void Controller::ProcessControllerInput()
	{
		m_pImpl->ProcessInputImpl();
	}

	bool Controller::IsDownThisFrame(ControllerButton button) const
	{
		return m_pImpl->IsDownThisFrameImpl(button);
	}

	bool Controller::IsUpThisFrame(ControllerButton button) const
	{
		return m_pImpl->IsUpThisFrameImpl(button);
	}
	bool Controller::IsPressed(ControllerButton button) const
	{
		return m_pImpl->IsPressedImpl(button);
	}

	void Controller::AddCommand(const std::shared_ptr<Command>& pCommand, ControllerButton button, KeyState keyState)
	{
		m_pImpl->AddCommandImpl(pCommand, button, keyState);
	}

	void Controller::RemoveCommand(ControllerButton button)
	{
		m_pImpl->RemoveCommandImpl(button);
	}

	void Controller::RemoveAllCommands()
	{
		m_pImpl->RemoveAllCommandsImpl();
	}

	glm::vec2 Controller::GetJoystickValue(bool leftJoystick)
	{
		return m_pImpl->GetJoystickValueImpl(leftJoystick);
	}

	float Controller::GetTriggerValue(bool leftTrigger)
	{
		return m_pImpl->GetTriggerValueImpl(leftTrigger);
	}
	
}