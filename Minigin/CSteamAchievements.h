#ifndef STEAMACHIEVEMENT_H
#define STEAMACHIEVEMENT_H

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
#pragma warning (push)
#pragma warning (disable: 4996)
#include "steam_api.h"
#pragma warning (pop)

struct Achievement_t
{
	int m_eAchievementID;
	const char* m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};
enum EAchievements
{
	ACH_WIN_ONE_GAME = 0,
	ACH_WIN_100_GAMES = 1,
	ACH_TRAVEL_FAR_ACCUM = 2,
	ACH_TRAVEL_FAR_SINGLE = 3,
};
class CSteamAchievements final
{

public:
	CSteamAchievements(Achievement_t* Achievements, int NumAchievements);
	virtual ~CSteamAchievements();

	CSteamAchievements(const CSteamAchievements&) = delete;
	CSteamAchievements(CSteamAchievements&&) noexcept = delete;
	CSteamAchievements& operator= (const CSteamAchievements&) = delete;
	CSteamAchievements& operator= (CSteamAchievements&&) noexcept = delete;

	bool RequestStats();
	bool SetAchievement(const char* ID);

	STEAM_CALLBACK(CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
	STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
	STEAM_CALLBACK(CSteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);

private:
	int64 m_iAppID; // Our current AppID
	Achievement_t* m_pAchievements; // Achievements data
	int m_iNumAchievements; // The number of Achievements
	bool m_bInitialized; // Have we called Request stats and received the callback?
};



#endif // !STEAMACHIEVEMENT_H
