#include "Achievement.h"

namespace dae
{

	std::unique_ptr<CSteamAchievements> Achievement::m_pSteamAchievements = NULL;

	Achievement_t Achievement::g_Achievements[4]{
			_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
			_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
			_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
			_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter")
	};

	
	void Achievement::Notify(PickUpComponent* pPickUpComponent)
	{
		m_Score += pPickUpComponent->GetScoreValue();
		if (m_Score >= 500)
		{
			if (m_pSteamAchievements)
				m_pSteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
		}
	}

	void Achievement::SetSteamAchievements(CSteamAchievements* pSteamAchievements)
	{
		m_pSteamAchievements.reset(pSteamAchievements);
	}

}
