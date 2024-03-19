#pragma once
#include <cstdint>
#include <memory>
#include <glm/glm.hpp>
#include "Command.h"


namespace dae
{
	struct ControllerButtons
	{
		static const int DpadUp;
		static const int DpadDown;
		static const int DpadLeft;
		static const int DpadRight;
		static const int Start;
		static const int Back;
		static const int LeftThumb;
		static const int RightThumb;
		static const int LeftShoulder;
		static const int RightShoulder;
		static const int A;
		static const int B;
		static const int X;
		static const int Y;
	};

	enum class KeyState;
	class Controller
	{
	public:

		Controller(uint8_t controllerIndex);

		~Controller();
		Controller(const Controller&) = delete;

		Controller(Controller&&) noexcept = default;

		Controller& operator= (const Controller&) = delete;
		Controller& operator= (Controller&&) noexcept = default;

		void ProcessControllerInput();
		bool IsDownThisFrame(int button) const;
		bool IsUpThisFrame(int button)  const;
		bool IsPressed(int button)  const;

		glm::vec2 GetJoystickValue(bool leftJoystick);
		float GetTriggerValue(bool leftTrigger);
	private:

		class ControllerImpl;
		ControllerImpl* m_pImpl;
	};
}

	
