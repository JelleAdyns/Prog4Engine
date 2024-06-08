#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include <Component.h>
#include <Command.h>
#include <memory>

class TimerComponent final : public dae::Component
{
public:
	explicit TimerComponent(dae::GameObject* pOwner, float timeToExecute, std::unique_ptr<dae::Command>& pCommand);
	virtual ~TimerComponent() = default;

	TimerComponent(const TimerComponent&) = delete;
	TimerComponent(TimerComponent&&) noexcept = delete;
	TimerComponent& operator= (const TimerComponent&) = delete;
	TimerComponent& operator= (TimerComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void Activate();

private:

	bool m_IsActive;

	float m_Timer{};
	float m_TimeToExecute;
	std::unique_ptr<dae::Command> m_pCommandToExecute;

};
#endif // !TIMERCOMPONENT_H
