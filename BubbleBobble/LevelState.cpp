#include "LevelState.h"
#include "LevelLoader.h"
#include "Components.h"
#include <SceneManager.h>


dae::GameObject* LevelState::m_pPlayerOne{ nullptr };
dae::GameObject* LevelState::m_pPlayerTwo{ nullptr };
const std::string LevelState::m_SceneName{ "Level" };

void LevelState::OnEnter()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_SceneName);
	auto pPlayer = std::make_unique<dae::GameObject>(38.f, 50.f);
	MakePlayer(pPlayer); 
	levelLoader::LoadLevel(m_LevelFile, m_LevelNumber);
	scene.AddGameObject(std::move(pPlayer));
	
	auto& ss2 = dae::AudioLocator::GetAudioService();
	ss2.PlaySoundClip(static_cast<dae::SoundID>(Game::SoundEvent::MainTheme), 80, true);
}

void LevelState::OnExit()
{
}

void LevelState::OnSuspend()
{
}

void LevelState::OnResume()
{
}

void LevelState::MakePlayer(const std::unique_ptr<dae::GameObject>& pPlayer)
{

	pPlayer->AddRenderComponent();
	pPlayer->AddPhysicsComponent();
	pPlayer->AddComponent<MovementComponent>(-160.f, 60.f);
	dae::PhysicsComponent::SetGravity(300);
	pPlayer->AddComponent<PlayerComponent>();
	PlayerComponent* playerComp = pPlayer->GetComponent<PlayerComponent>();
	pPlayer->AddComponent<SpriteComponent>("Textures/BubStates.png", 4, 8, 0.1f, true, false);
	SpriteComponent* spriteComp = pPlayer->GetComponent<SpriteComponent>();
	spriteComp->AddObserver(playerComp);
	spriteComp->SetHeightMarkers(0, HitState::GetHitSpriteOffset());

	const auto& destRctSize = spriteComp->GetDestRectSize();

	pPlayer->AddComponent<dae::CollisionComponent>(
		glm::vec2{ m_GeneralCollisionOffset ,m_GeneralCollisionOffset },
		glm::vec2{ destRctSize.x - m_GeneralCollisionOffset * 2 ,destRctSize.y - m_GeneralCollisionOffset * 2 },
		collisionTags::playerTag);

	pPlayer->AddComponent<WallCheckingComponent>(glm::vec2{ 0,destRctSize.y / 4 }, glm::vec2{ destRctSize.x,destRctSize.y / 2 });
	pPlayer->AddComponent<FloorCheckingComponent>(glm::vec2{ destRctSize.x / 4,0 }, glm::vec2{ destRctSize.x / 2,destRctSize.y });

	MovementComponent* movementComp = pPlayer->GetComponent<MovementComponent>();

	if(movementComp->GetPlayerIndex() == 0) m_pPlayerOne = pPlayer.get();
	else m_pPlayerTwo = pPlayer.get();

}
