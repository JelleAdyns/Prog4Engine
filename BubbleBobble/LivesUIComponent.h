#ifndef LIVESOBSERVER_H
#define LIVESOBSERVER_H

#include "Observer.h"
#include "Component.h"
#include <string>
#include <vector>


namespace dae
{
	class GameObject;
}

class PlayerComponent;
class LivesUIComponent final : public dae::Component, public dae::Observer<PlayerComponent>
{
public:
	explicit LivesUIComponent(dae::GameObject* pOwner);
	virtual ~LivesUIComponent();

	LivesUIComponent(const LivesUIComponent&) = delete;
	LivesUIComponent(LivesUIComponent&&) noexcept = delete;
	LivesUIComponent& operator= (const LivesUIComponent&) = delete;
	LivesUIComponent& operator= (LivesUIComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(PlayerComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<PlayerComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid(dae::Subject<PlayerComponent>* pSubject) override;

	void AddLifeObjct(dae::GameObject* pLife);
	int GetRemainingLives() const { return static_cast<int>(m_pLives.size()); };


private:

    std::vector<dae::GameObject*> m_pLives;

    std::vector<dae::Subject<PlayerComponent>*> m_pVecObservedSubjects;
};

#endif // !LIVESOBSERVER_H