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


		bool ProcessInputImpl();
		bool IsDownThisFrameImpl(int button) const;
		bool IsUpThisFrameImpl(int button)  const;
		bool IsPressedImpl(int button)  const;

		void AddCommandImpl(std::unique_ptr<Command>&& pCommand, int button, KeyState keyState);
	private:
		uint8_t m_ControllerIndex{};
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedThisFrame{};

		std::unordered_map<int, std::pair<std::unique_ptr<Command>, KeyState>> m_MapCommands{};

		void HandleInput() const;
	};

	void Controller::ControllerImpl::HandleInput() const
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

	bool Controller::ControllerImpl::ProcessInputImpl()
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
	void Controller::ControllerImpl::AddCommandImpl(std::unique_ptr<Command>&& pCommand, int button, KeyState keyState)
	{
		m_MapCommands[button] = std::make_pair(std::move(pCommand), keyState);
	}

	//Controller
	Controller::Controller(uint8_t controllerIndex) : 
		m_pImpl{ new ControllerImpl{controllerIndex} }
	{}

	Controller::~Controller()
	{
		delete m_pImpl;
	}

	

	bool Controller::ProcessInput()
	{
		return m_pImpl->ProcessInputImpl();
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
	void Controller::AddCommand(std::unique_ptr<Command>&& pCommand, int button, KeyState keyState)
	{
		m_pImpl->AddCommandImpl(std::move(pCommand),button,keyState);
	}
	
}