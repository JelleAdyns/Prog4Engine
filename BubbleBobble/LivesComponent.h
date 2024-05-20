#ifndef LIVESCOMPONENT_H
#define LIVESCOMPONENT_H

#include "Component.h"
#include "Subject.h"

namespace dae
{
	class GameObject;
}

class LivesUIComponent;
class LivesComponent final : public dae::Component
{
public:
	explicit LivesComponent(dae::GameObject* pOwner, int nrOfLives, LivesUIComponent* pObserver);
	virtual ~LivesComponent() = default;

	LivesComponent(const LivesComponent&) = delete;
	LivesComponent(LivesComponent&&) noexcept = delete;
	LivesComponent& operator= (const LivesComponent&) = delete;
	LivesComponent& operator= (LivesComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;
	void TakeLife();
	int GetNrOfLives() const;

private:
	int m_Lives;
	std::unique_ptr<dae::Subject<LivesComponent>> m_LivesChanged;
};


#endif // !LIVESCOMPONENT_H

