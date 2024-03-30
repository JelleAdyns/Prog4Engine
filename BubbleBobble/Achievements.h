#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H
//#include "CSteamAchievements.h"
#include "Observer.h"
#include "ScoreUIComponent.h"
#include "Singleton.h"

namespace dae
{
	struct AchievementProps
	{
		const char* name;
		bool achieved;
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

		//static void SetSteamAchievements(CSteamAchievements* pSteamAchievements);
		//static Achievement_t g_Achievements[4];
		static AchievementProps g_Achievements[2];
	private:
		friend class Singleton<Achievements>;
		Achievements() :
			Observer{}
		{}
		
		//static std::unique_ptr<CSteamAchievements> m_pSteamAchievements;
	};
}
#endif // !ACHIEVEMENT_H