#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstdint>
#include <memory>
#include <glm/glm.hpp>
#include "Command.h"
#include "Windows.h"
#include <XInput.h>


namespace dae
{
	
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

	enum class KeyState;
	class Controller final
	{
	public:

		Controller(uint8_t controllerIndex);

		~Controller();
		Controller(const Controller&) = delete;

		Controller(Controller&&) noexcept = default;

		Controller& operator= (const Controller&) = delete;
		Controller& operator= (Controller&&) noexcept = default;

		bool IsAnyButtonPressed();
		void ProcessControllerInput();
		bool IsDownThisFrame(ControllerButton button) const;
		bool IsUpThisFrame(ControllerButton button)  const;
		bool IsPressed(ControllerButton button)  const;

		void AddCommand(const std::shared_ptr<Command>& pCommand, ControllerButton button, KeyState keyState);
		void RemoveCommand(ControllerButton button, KeyState keyState);
		void RemoveAllCommands();

		void Vibrate(int strengthPrecantage);
		glm::vec2 GetJoystickValue(bool leftJoystick);
		float GetTriggerValue(bool leftTrigger);
		static int AmountOfConnectedControllers();
	private:

		class ControllerImpl;
		ControllerImpl* m_pImpl;
	};
}

#endif // !CONTROLLER_H