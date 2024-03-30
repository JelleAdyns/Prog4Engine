#ifndef LIVESOBSERVER_H
#define LIVESOBSERVER_H

#include "Observer.h"
#include "Component.h"
#include <string>
namespace dae
{
	class TextComponent;
	class GameObject;
	class LivesComponent;
	class LivesUIComponent final : public Component, public Observer<LivesComponent>
	{
	public:
		explicit LivesUIComponent(GameObject* pOwner);
		virtual ~LivesUIComponent() = default;

		LivesUIComponent(const LivesUIComponent&) = delete;
		LivesUIComponent(LivesUIComponent&&) noexcept = delete;
		LivesUIComponent& operator= (const LivesUIComponent&) = delete;
		LivesUIComponent& operator= (LivesUIComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		virtual void Notify(LivesComponent* pSubject) override;
	private:

		TextComponent* m_pTextComponent;
	};

}

#endif // !LIVESOBSERVER_H