#ifndef KEYSTATE_H
#define KEYSTATE_H

namespace dae
{
	enum class KeyState
	{
		DownThisFrame,
		UpThisFrame,
		Pressed,
		NotPressed

	};
}
#endif // !KEYSTATE_H