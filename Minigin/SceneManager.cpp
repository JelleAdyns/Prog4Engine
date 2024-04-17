#include "SceneManager.h"
#include "Scene.h"
#include "Minigin.h"
#include <iostream>
#include <SDL_opengl.h>

void dae::SceneManager::Update()
{
	m_pMapScenes.at(m_ActiveScene)->Update();
}

void dae::SceneManager::PrepareImGuiRender()
{
	m_pMapScenes.at(m_ActiveScene)->PrepareImGuiRender();
}

void dae::SceneManager::Render() const
{
	//auto scale = Minigin::GetWindowScale();
	//glPushMatrix();
	//glScalef(float(scale), float(scale), 0);
	m_pMapScenes.at(m_ActiveScene)->Render();
	//glPopMatrix();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& pScene = std::shared_ptr<Scene>{ new Scene{} };

	if (m_pMapScenes.contains(name)) std::cout << "This name for the new scene already exists, overwriting now.\n";
	m_pMapScenes[name] = pScene;
	return *pScene;
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	if(m_pMapScenes.contains(name)) m_pMapScenes.erase(name);
}

void dae::SceneManager::SetActiveScene(const std::string& sceneToActivate)
{
	if (m_pMapScenes.contains(sceneToActivate)) m_ActiveScene = sceneToActivate;
}
