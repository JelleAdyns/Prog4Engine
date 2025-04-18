#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstdint>
#include <memory>
#include <glm/glm.hpp>
#include "Command.h"



namespace dae
{

	enum class ControllerButton 
	{
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000
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

		void DeactivateAllCommands();
		void ActivateAllCommands();

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