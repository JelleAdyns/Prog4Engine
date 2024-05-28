#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H


#include "Component.h"
#include "SpriteComponent.h"
#include <Observer.h>

namespace dae
{
	class GameObject;
	class PhysicsComponent;
}

class WallCheckingComponent;
class ProjectileComponent final : public dae::Component, public dae::Observer<SpriteComponent>
{
public:
	explicit ProjectileComponent(dae::GameObject* pOwner, bool left);
	virtual ~ProjectileComponent();

	ProjectileComponent(const ProjectileComponent&) = delete;
	ProjectileComponent(ProjectileComponent&&) noexcept = delete;
	ProjectileComponent& operator= (const ProjectileComponent&) = delete;
	ProjectileComponent& operator= (ProjectileComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	virtual void Notify(SpriteComponent* pSubject) override;
	virtual void AddSubjectPointer(dae::Subject<SpriteComponent>* pSubject) override;
	virtual void SetSubjectPointersInvalid() override;

private:

	static constexpr float m_Speed{ 70.f };
	bool m_IsBroken{ false };

	const float m_Velocity;

	SpriteComponent* m_pSpriteComp;
	WallCheckingComponent* m_pWallComp;
	dae::PhysicsComponent* m_pPhysicsComp;

	std::vector<dae::Subject<SpriteComponent>*> m_pVecObservedSubjects;
};

#endif // !PROJECTILECOMPONENT_H

