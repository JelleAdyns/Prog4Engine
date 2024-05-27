#ifndef LIVESOBSERVER_H
#define LIVESOBSERVER_H

#include "Observer.h"
#include "Component.h"
#include <string>
#include <vector>


namespace dae
{
    template <typename T>
    class Subject;
	class TextComponent;
	class GameObject;
}

class LivesComponent;
class LivesUIComponent final : public dae::Component, public dae::Observer<LivesComponent>
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

	virtual void Notify(LivesComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<LivesComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

private:

	dae::TextComponent* m_pTextComponent;

    std::vector<dae::Subject<LivesComponent>*> m_pVecObservedSubjects;
};

#endif // !LIVESOBSERVER_H