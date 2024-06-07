#include "Achievements.h"
#include <algorithm>


/*std::unique_ptr<CSteamAchievements> Achievements::m_pSteamAchievements = NULL;

Achievement_t Achievements::g_Achievements[4]{
		_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
		_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
		_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter")
};*/

AchievementProps Achievements::g_Achievements[3]{
	{"Score500Points", false},
	{"WinOneGame", false},
	{"WinAHundredGames", false}
};

Achievements::~Achievements()
{
    for (auto& pSubject : m_pVecObservedSubjects)
    {
        if(pSubject) pSubject->RemoveObserver(this);
    }
}

void Achievements::Notify(ScoreUIComponent* pScoreUIComponent)
{
	if (!g_Achievements[0].achieved && pScoreUIComponent->GetScore() >= 500)
	{
		std::cout << g_Achievements[0].name << '\n';
		g_Achievements[0].achieved = true;
	}
}

void Achievements::AddSubjectPointer(dae::Subject<ScoreUIComponent>* pScoreUIComponent)
{
    m_pVecObservedSubjects.emplace_back(pScoreUIComponent);
}

void Achievements::SetSubjectPointersInvalid(dae::Subject<ScoreUIComponent>* pScoreUIComponent)
{
	auto pos = std::find(m_pVecObservedSubjects.begin(), m_pVecObservedSubjects.end(), pScoreUIComponent);
	if (pos != m_pVecObservedSubjects.cend())
	{
		m_pVecObservedSubjects.erase(pos);
	}
}

/*void Achievements::SetSteamAchievements(CSteamAchievements* pSteamAchievements)
{
	m_pSteamAchievements.reset(pSteamAchievements);
}*/

