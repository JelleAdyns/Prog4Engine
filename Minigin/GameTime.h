#ifndef GAMETIME_H
#define GAMETIME_H


#include "Singleton.h"

namespace dae
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		virtual ~GameTime() = default;
		GameTime(const GameTime& other) = delete;
		GameTime(GameTime&& other) noexcept = delete;
		GameTime& operator=(const GameTime& other) = delete;
		GameTime& operator=(GameTime&& other) noexcept = delete;

		void Init(float fixedTimeStep, int FPSGoal) { m_DeltaTime = 0; m_TotalTime = 0; m_FixedTimeStep = fixedTimeStep; m_MillisecPerFrame = 1000.f / FPSGoal; }

		float GetFixedTimeStep() { return m_FixedTimeStep; }
		float GetDeltaTime() { return m_DeltaTime; }
		float GetTotalTime() { return m_TotalTime; }
		float GetMillisecPerFrame() { return m_MillisecPerFrame; }
		bool UsingFPSGoal() { return m_UseFPSGoal; }

		void SetDeltaTime(float newDeltaTime) { m_DeltaTime = newDeltaTime; m_TotalTime += newDeltaTime; }
		void ToggleUseFPSGoal() { m_UseFPSGoal = !m_UseFPSGoal; }

	private:
		friend class Singleton<GameTime>;
		GameTime() = default;

		float m_FixedTimeStep = 0.02f;
		float m_DeltaTime = 0.0f;
		float m_TotalTime = 0.0f;
		float m_MillisecPerFrame = 1000.f / 60.f;
		bool m_UseFPSGoal = false;
	};
}

#endif // !GAMETIME_H