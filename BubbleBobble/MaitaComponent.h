#ifndef MAITACOMPONENT_H
#define MAITACOMPONENT_H
//
//#include <Component.h>
//#include <Subject.h>
//#include "EnemyState.h"
//
//namespace dae
//{
//	class PhysicsComponent;
//}
//class PlayerComponent;
//class SpriteComponent;
//class MaitaComponent final : public dae::Component
//{
//public:
//
//	explicit MaitaComponent(dae::GameObject* pOwner);
//	virtual ~MaitaComponent() = default;
//	MaitaComponent(const MaitaComponent&) = delete;
//	MaitaComponent(MaitaComponent&&) noexcept = delete;
//	MaitaComponent& operator= (const MaitaComponent&) = delete;
//	MaitaComponent& operator= (MaitaComponent&&) noexcept = delete;
//
//	virtual void Start() override;
//	virtual void Update() override;
//	virtual void PrepareImGuiRender() override;
//
//	void AddPlayerObserver(PlayerComponent* pSubject);
//
//	const std::vector<dae::Subject<PlayerComponent>*>& GetPlayerSubjects() const { return m_SubjectsForState; }
//
//	static float GetMaitaOffset() { return m_MaitaOffset; }
//
//private:
//	void UpdateStates();
//
//	static constexpr float m_MaitaOffset{ 8.f };
//
//	std::unique_ptr<EnemyState> m_pCurrState;
//
//	dae::PhysicsComponent* m_pPhysicsComp;
//	SpriteComponent* m_pSpriteComp;
//
//	std::vector<dae::Subject<PlayerComponent>*> m_SubjectsForState;
//};
//


#endif // !MAITACOMPONENT_H
