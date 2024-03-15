#pragma once
#include <Windows.h>
#include <XInput.h>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include "Command.h"

namespace dae
{
	enum class KeyState;
	enum class ControllerButton
	{
		DpadUp = XINPUT_GAMEPAD_DPAD_UP,
		DpadDown = XINPUT_GAMEPAD_DPAD_DOWN,
		DpadLeft = XINPUT_GAMEPAD_DPAD_LEFT,
		DpadRight = XINPUT_GAMEPAD_DPAD_RIGHT,
		Start = XINPUT_GAMEPAD_START,
		Back = XINPUT_GAMEPAD_BACK,
		LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y
	};

	class Controller
	{
	public:
		Controller(uint8_t controllerIndex) : m_ControllerIndex{ controllerIndex }
		{};

		~Controller() = default;
		Controller(const Controller&) = delete;

		Controller(Controller&&) noexcept = default;

		Controller& operator= (const Controller&) = delete;
		Controller& operator= (Controller&&) noexcept = delete;

		bool ProcessInput();
		void HandleInput() const;
		bool IsDownThisFrame(ControllerButton button) const;
		bool IsUpThisFrame(ControllerButton button)  const;
		bool IsPressed(ControllerButton button)  const;

		void AddCommand(std::unique_ptr<Command>&& pCommand, ControllerButton button, KeyState keyState);

	private:
		uint8_t m_ControllerIndex{};
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedThisFrame{};

		std::unordered_map<ControllerButton, std::pair<std::unique_ptr<Command>, KeyState>> m_MapCommands{};
	};
}

	
