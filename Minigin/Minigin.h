#ifndef MINIGIN_H
#define MINIGIN_H

#include <string>
#include <functional>
#include <memory>
#include <glm/vec2.hpp>

struct SDL_Window;
namespace dae
{
	class Minigin final
	{
	public:
		explicit Minigin(const std::string& dataPath, int windowScale, float fixedTimeStep, int FPSGoal);
		~Minigin();
		void Run();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		static glm::ivec2 GetWindowSize();
		static int GetWindowScale();

	private:
		static int m_WindowScale;
		static const glm::ivec2 m_WindowSize;
		static SDL_Window* m_Window;
	};
}

#endif // !MINIGIN_H