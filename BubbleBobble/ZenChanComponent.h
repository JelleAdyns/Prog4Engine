#ifndef ZENCHANCOMPONENT_H
#define ZENCHANCOMPONENT_H
//
//#include <Component.h>
//#include <Subject.h>
//#include "States.h"
//
//namespace dae
//{
//	class PhysicsComponent;
//}
//class PlayerComponent;
//class ZenChanComponent final : public dae::Component
//{
//public:
//
//	explicit ZenChanComponent(dae::GameObject* pOwner);
//	virtual ~ZenChanComponent() = default;
//	ZenChanComponent(const ZenChanComponent&) = delete;
//	ZenChanComponent(ZenChanComponent&&) noexcept = delete;
//	ZenChanComponent& operator= (const ZenChanComponent&) = delete;
//	ZenChanComponent& operator= (ZenChanComponent&&) noexcept = delete;
//
//	virtual void Start() override;
//	virtual void Update() override;
//	virtual void PrepareImGuiRender() override;
//
//	void AddPlayerObserver(PlayerComponent* pSubject);
//
//	const std::vector<dae::Subject<PlayerComponent>*>& GetPlayerSubjects() const { return m_SubjectsForState; }
//
//private:
//	void UpdateStates();
//
//	std::unique_ptr<EnemyState> m_pCurrState;
//
//	dae::PhysicsComponent* m_pPhysicsComp;
//	SpriteComponent* m_pSpriteComp;
//
//	std::vector<dae::Subject<PlayerComponent>*> m_SubjectsForState;
//};
//


#endif // !ZENCHANCOMPONENT_H
