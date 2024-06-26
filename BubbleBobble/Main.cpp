//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "Game.h"

int main(int, char* []) {

	/*if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
	{
		dae::Achievements::GetInstance().SetSteamAchievements( new CSteamAchievements(dae::Achievements::g_Achievements, 4));
		std::cout << "Successfully initialized steam." << std::endl;
	}*/

	dae::Minigin engine{ "../Data/",3, 0.02f, 60 };
	
	Game::GetInstance().StartGame();

	engine.Run();

	// Shutdown Steam
	//SteamAPI_Shutdown();

    return 0;
}