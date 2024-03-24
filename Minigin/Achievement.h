#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H
#include "CSteamAchievements.h"
#include "Observer.h"
#include "PickUpComponent.h"
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
	class Achievement final : public Singleton<Achievement>, public Observer<PickUpComponent>
	{
	public:
		


		
		virtual ~Achievement() = default;

		Achievement(const Achievement&) = delete;
		Achievement(Achievement&&) noexcept = delete;
		Achievement& operator= (const Achievement&) = delete;
		Achievement& operator= (Achievement&&) noexcept = delete;
		
		virtual void Notify(PickUpComponent* pPickUpComponent) override;

		static void SetSteamAchievements(CSteamAchievements* pSteamAchievements);
		static Achievement_t g_Achievements[4];
	private:
		friend class Singleton<Achievement>;
		Achievement() :
			Observer{},
			m_Score{}
		{}

		int m_Score;
		
		static std::unique_ptr<CSteamAchievements> m_pSteamAchievements;
	};
}
#endif // !ACHIEVEMENT_H