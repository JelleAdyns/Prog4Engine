#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H


#include <Component.h>
#include <Subject.h>
#include "PlayerComponent.h"
#include "PickUpComponent.h"

class SpriteComponent;
class ScoreComponent final : public dae::Component
{
public:

	explicit ScoreComponent(dae::GameObject* pOwner, PickUpComponent::PickUpType pickUpType, PlayerComponent::PlayerType playerType);
	virtual ~ScoreComponent() = default;


	ScoreComponent(const ScoreComponent&) = delete;
	ScoreComponent(ScoreComponent&&) noexcept = delete;
	ScoreComponent& operator= (const ScoreComponent&) = delete;
	ScoreComponent& operator= (ScoreComponent&&) noexcept = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void PrepareImGuiRender() override;


private:

	const int m_Score;
	PlayerComponent::PlayerType m_PlayerWhoScored;
	
	float m_Timer{};
	const float m_MaxTimeAlive{ 1.f };

	SpriteComponent* m_pSpriteComp;

};
#endif // !SCORECOMPONENT_H