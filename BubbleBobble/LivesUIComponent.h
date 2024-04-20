#ifndef LIVESOBSERVER_H
#define LIVESOBSERVER_H

#include "Observer.h"
#include "Component.h"
#include <string>

class dae::TextComponent;
class dae::GameObject;
class LivesComponent;
class LivesUIComponent final : public dae::Component, public dae::Observer<LivesComponent>
{
public:
	explicit LivesUIComponent(dae::GameObject* pOwner);
	virtual ~LivesUIComponent() = default;

	LivesUIComponent(const LivesUIComponent&) = delete;
	LivesUIComponent(LivesUIComponent&&) noexcept = delete;
	LivesUIComponent& operator= (const LivesUIComponent&) = delete;
	LivesUIComponent& operator= (LivesUIComponent&&) noexcept = delete;

	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(LivesComponent* pSubject) override;
private:

	dae::TextComponent* m_pTextComponent;
};

#endif // !LIVESOBSERVER_H