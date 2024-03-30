#ifndef LIVESCOMPONENT_H
#define LIVESCOMPONENT_H

#include "Component.h"
#include "Subject.h"
namespace dae
{
	class GameObject;
	class LivesUIComponent;
	class LivesComponent final : public Component
	{
	public:
		explicit LivesComponent(GameObject* pOwner, int nrOfLives, LivesUIComponent* pObserver);
		virtual ~LivesComponent() = default;

		LivesComponent(const LivesComponent&) = delete;
		LivesComponent(LivesComponent&&) noexcept = delete;
		LivesComponent& operator= (const LivesComponent&) = delete;
		LivesComponent& operator= (LivesComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;
		void TakeLife();
		int GetNrOfLives() const;

	private:
		int m_Lives;
		std::unique_ptr<Subject<LivesComponent>> m_LivesChanged;
	};
}

#endif // !LIVESCOMPONENT_H

