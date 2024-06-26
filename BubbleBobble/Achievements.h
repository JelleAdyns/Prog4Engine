#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H
//#include "CSteamAchievements.h"
#include "Observer.h"
#include "ScoreUIComponent.h"
#include "Singleton.h"

struct AchievementProps
{
	const char* name;
	bool achieved;
};
	
class Achievements final : public dae::Singleton<Achievements>, public dae::Observer<ScoreUIComponent>
{
public:
		
	virtual ~Achievements();

	Achievements(const Achievements&) = delete;
	Achievements(Achievements&&) noexcept = delete;
	Achievements& operator= (const Achievements&) = delete;
	Achievements& operator= (Achievements&&) noexcept = delete;
		
	virtual void Notify(ScoreUIComponent* pScoreUIComponent) override;
	virtual void AddSubjectPointer(dae::Subject<ScoreUIComponent>* pScoreUIComponent) override;
	virtual void SetSubjectPointersInvalid(dae::Subject<ScoreUIComponent>* pScoreUIComponent) override;

	//static void SetSteamAchievements(CSteamAchievements* pSteamAchievements);
	//static Achievement_t g_Achievements[4];
	static AchievementProps g_Achievements[3];
private:
	friend class dae::Singleton<Achievements>;
	Achievements() :
		dae::Observer<ScoreUIComponent>{}
	{}
		
    std::vector<dae::Subject<ScoreUIComponent>*> m_pVecObservedSubjects{};
	//static std::unique_ptr<CSteamAchievements> m_pSteamAchievements;
};

#endif // !ACHIEVEMENT_H