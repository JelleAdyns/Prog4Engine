#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_pVecScenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::PrepareImGuiRender()
{
	for (const auto& scene : m_pVecScenes)
	{
		scene->PrepareImGuiRender();
	}
}

void dae::SceneManager::Render() const
{
	for (const auto& scene : m_pVecScenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_pVecScenes.push_back(scene);
	return *scene;
}
