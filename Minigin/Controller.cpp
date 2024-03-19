#include "Controller.h"
#include "KeyState.h"
#include <unordered_map>
#include <Windows.h>
#include <XInput.h>

namespace dae
{

	const int ControllerButtons::DpadUp = XINPUT_GAMEPAD_DPAD_UP;
	const int ControllerButtons::DpadDown = XINPUT_GAMEPAD_DPAD_DOWN;
	const int ControllerButtons::DpadLeft = XINPUT_GAMEPAD_DPAD_LEFT;
	const int ControllerButtons::DpadRight = XINPUT_GAMEPAD_DPAD_RIGHT;
	const int ControllerButtons::Start = XINPUT_GAMEPAD_START;
	const int ControllerButtons::Back = XINPUT_GAMEPAD_BACK;
	const int ControllerButtons::LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB;
	const int ControllerButtons::RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB;
	const int ControllerButtons::LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER;
	const int ControllerButtons::RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	const int ControllerButtons::A = XINPUT_GAMEPAD_A;
	const int ControllerButtons::B = XINPUT_GAMEPAD_B;
	const int ControllerButtons::X = XINPUT_GAMEPAD_X;
	const int ControllerButtons::Y = XINPUT_GAMEPAD_Y;
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


		void ProcessInputImpl();
		bool IsDownThisFrameImpl(int button) const;
		bool IsUpThisFrameImpl(int button)  const;
		bool IsPressedImpl(int button)  const;

		glm::vec2 GetJoystickValueImpl(bool leftJoystick);
		float GetTriggerValueImpl(bool leftJoystick);


	private:
		uint8_t m_ControllerIndex{};
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedThisFrame{};

		static float m_MaxJoystickValue;
		static float m_JoystickDeadZonePercentage;
		static float m_MaxTriggerValue;
		static float m_TriggerDeadZonePercentage;
	};


	float Controller::ControllerImpl::m_MaxJoystickValue = static_cast<float>(SHRT_MAX);
	float Controller::ControllerImpl::m_JoystickDeadZonePercentage = 30.f;
	float Controller::ControllerImpl::m_MaxTriggerValue = static_cast<float>(_UI8_MAX);
	float Controller::ControllerImpl::m_TriggerDeadZonePercentage = 50.f;


	void Controller::ControllerImpl::ProcessInputImpl()
	{
		m_PreviousState = m_CurrentState;
		m_CurrentState = XINPUT_STATE{};
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

		//XINPUT_VIBRATION vibration;
		//ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		//vibration.wLeftMotorSpeed = 32000; // use any value between 0-65535 here
		//vibration.wRightMotorSpeed = 16000; // use any value between 0-65535 here
		//XInputSetState(m_ControllerIndex, &vibration);

	}
	bool Controller::ControllerImpl::IsDownThisFrameImpl(int button) const
	{
		return m_ButtonsPressedThisFrame & static_cast<int>(button);
	}
	bool Controller::ControllerImpl::IsUpThisFrameImpl(int button) const
	{
		return m_ButtonsReleasedThisFrame & static_cast<int>(button);
	}
	bool Controller::ControllerImpl::IsPressedImpl(int button) const
	{
		return m_CurrentState.Gamepad.wButtons & static_cast<int>(button);
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

	

	void Controller::ProcessControllerInput()
	{
		m_pImpl->ProcessInputImpl();
	}

	bool Controller::IsDownThisFrame(int button) const
	{
		return m_pImpl->IsDownThisFrameImpl(button);
	}

	bool Controller::IsUpThisFrame(int button) const
	{
		return m_pImpl->IsUpThisFrameImpl(button);
	}
	bool Controller::IsPressed(int button) const
	{
		return m_pImpl->IsPressedImpl(button);
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