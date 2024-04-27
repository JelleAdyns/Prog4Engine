#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Minigin.h"
#include "InputCommandBinder.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include <chrono>
#include <thread>

//#pragma warning(push)
//#pragma warning (disable: 4996)
//#include <steam_api.h>
//#pragma warning(pop)

namespace dae
{
	int Minigin::m_WindowScale{ 1 };
	const glm::ivec2 Minigin::m_WindowSize{ 256, 224 };
	SDL_Window* Minigin::m_Window{ nullptr };

	void PrintSDLVersion()
	{
		SDL_version version{};
		SDL_VERSION(&version);
		printf("We compiled against SDL version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

		SDL_GetVersion(&version);
		printf("We are linking against SDL version %u.%u.%u.\n",
			version.major, version.minor, version.patch);

		SDL_IMAGE_VERSION(&version);
		printf("We compiled against SDL_image version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

		version = *IMG_Linked_Version();
		printf("We are linking against SDL_image version %u.%u.%u.\n",
			version.major, version.minor, version.patch);

		SDL_TTF_VERSION(&version)
			printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
				version.major, version.minor, version.patch);

		version = *TTF_Linked_Version();
		printf("We are linking against SDL_ttf version %u.%u.%u.\n",
			version.major, version.minor, version.patch);

		SDL_MIXER_VERSION(&version)
			printf("We compiled against SDL_mixer version %u.%u.%u ...\n",
				version.major, version.minor, version.patch);

		version = *Mix_Linked_Version();
		printf("We are linking against SDL_mixer version %u.%u.%u.\n",
			version.major, version.minor, version.patch);
	}

	dae::Minigin::Minigin(const std::string& dataPath, int windowScale, float fixedTimeStep, int FPSGoal)
	{
		m_WindowScale = windowScale;

		PrintSDLVersion();

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			throw std::runtime_error(std::string("Error when calling Mix_OpenAudio: ") + SDL_GetError());
		}
		m_Window = SDL_CreateWindow(
			"Programming 4 assignment",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_WindowSize.x * windowScale,
			m_WindowSize.y * windowScale,
			SDL_WINDOW_OPENGL
		);
		if (m_Window == nullptr)
		{
			throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
		}

		Renderer::GetInstance().Init(m_Window);

		ResourceManager::GetInstance().Init(dataPath);

		GameTime::GetInstance().Init(fixedTimeStep, FPSGoal);
	}

	dae::Minigin::~Minigin()
	{
		Renderer::GetInstance().Destroy();
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
		SDL_Quit();
	}

	void dae::Minigin::Run()
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputCommandBinder::GetInstance();
		auto& gameTime = GameTime::GetInstance();

		bool doContinue = true;

		auto prevTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;
		while (doContinue)
		{
			const auto currTime = std::chrono::high_resolution_clock::now();
			gameTime.SetDeltaTime(std::chrono::duration<float>(currTime - prevTime).count());
			prevTime = currTime;
			lag += gameTime.GetDeltaTime();

			//SteamAPI_RunCallbacks();
			doContinue = input.ProcessInput();

			while (lag >= gameTime.GetFixedTimeStep())
			{
				sceneManager.FixedUpdate();
				lag -= gameTime.GetFixedTimeStep();
			}

			sceneManager.Update();
			renderer.Render();

			if (gameTime.UsingFPSGoal())
			{
				const auto sleepTime = currTime + std::chrono::milliseconds(int(gameTime.GetMillisecPerFrame())) - std::chrono::high_resolution_clock::now();
				std::this_thread::sleep_for(sleepTime);
			}

		}

	}
	glm::ivec2 Minigin::GetWindowSize()
	{
		return m_WindowSize;
	}
	int Minigin::GetWindowScale()
	{
		return m_WindowScale;
	}
}

