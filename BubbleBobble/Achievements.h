#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H
#include "CSteamAchievements.h"
#include "Observer.h"
#include "ScoreUIComponent.h"
#include "Singleton.h"

namespace dae
{
	enum EAchievements
		{
			ACH_WIN_ONE_GAME = 0,
			ACH_WIN_100_GAMES = 1,
			ACH_TRAVEL_FAR_ACCUM = 2,
			ACH_TRAVEL_FAR_SINGLE = 3,
		};
	class Achievements final : public Singleton<Achievements>, public Observer<ScoreUIComponent>
	{
	public:
		
		virtual ~Achievements() = default;

		Achievements(const Achievements&) = delete;
		Achievements(Achievements&&) noexcept = delete;
		Achievements& operator= (const Achievements&) = delete;
		Achievements& operator= (Achievements&&) noexcept = delete;
		
		virtual void Notify(ScoreUIComponent* pScoreUIComponent) override;

		static void SetSteamAchievements(CSteamAchievements* pSteamAchievements);
		static Achievement_t g_Achievements[4];
	private:
		friend class Singleton<Achievements>;
		Achievements() :
			Observer{}
		{}
		
		static std::unique_ptr<CSteamAchievements> m_pSteamAchievements;
	};
}
#endif // !ACHIEVEMENT_H