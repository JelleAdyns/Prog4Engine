#include "SceneManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Minigin.h"
#include <iostream>
#include <algorithm>

void dae::SceneManager::Start()
{
	m_pMapScenes.at(m_ActiveScene)->Start();
}

void dae::SceneManager::Update()
{
	m_pMapScenes.at(m_ActiveScene)->Update();

	CheckForDestroyedScenes();
}

void dae::SceneManager::PrepareImGuiRender()
{
	m_pMapScenes.at(m_ActiveScene)->PrepareImGuiRender();
}

void dae::SceneManager::Render() const
{
	m_pMapScenes.at(m_ActiveScene)->Render();
}

void dae::SceneManager::FixedUpdate()
{
	m_pMapScenes.at(m_ActiveScene)->FixedUpdate();

	CheckForDestroyedScenes();
}

void dae::SceneManager::CheckForDestroyedScenes()
{
	std::for_each(m_pMapScenes.begin(), m_pMapScenes.end(), [&](const std::pair<std::string, std::shared_ptr<Scene>>& pair)
		{
			if (pair.second->IsDestroyed()) m_KeysToDestroy.push_back(pair.first);
		});
	std::for_each(m_KeysToDestroy.cbegin(), m_KeysToDestroy.cend(), [&](const std::string& key)
		{
			m_pMapScenes.erase(key);
		});
	m_KeysToDestroy.clear();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& pScene = std::shared_ptr<Scene>{ new Scene{} };

	if (m_pMapScenes.contains(name)) std::cout << "This name ("<<name<<") for the new scene already exists, overwriting now.\n";
	m_pMapScenes[name] = pScene;
	SetActiveScene(name);
	return *pScene;
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	if(m_pMapScenes.contains(name)) m_pMapScenes.erase(name);
}

void dae::SceneManager::RemoveNonActiveScenes()
{
	 
	std::for_each(m_pMapScenes.cbegin(), m_pMapScenes.cend(), [&](const std::pair<std::string, std::shared_ptr<Scene>>& pair)
		{
			if(pair.first != m_ActiveScene)	pair.second->SetDestroyed();
		});
	
}

void dae::SceneManager::SetActiveScene(const std::string& sceneToActivate)
{
	if (m_pMapScenes.contains(sceneToActivate)) m_ActiveScene = sceneToActivate;
}
