#include "Scene.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene() { ++m_IdCounter; }

Scene::~Scene() { --m_IdCounter; }

void Scene::AddGameObject(std::unique_ptr<GameObject>&& object)
{
	m_pObjects.emplace_back(std::move(object));
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), object), m_pObjects.end());
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
}


void Scene::Update()
{
	for(auto& object : m_pObjects)
	{
		object->Update();
	}

	m_pObjects.erase(
		std::remove_if(m_pObjects.begin(), m_pObjects.end(),
			[&](const std::unique_ptr<GameObject>& pObject) {return pObject->IsDead(); }
		), 
		m_pObjects.cend()
	);
}

void dae::Scene::PrepareImGuiRender()
{
	for (auto& object : m_pObjects)
	{
		object->PrepareImGuiRender();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

Scene::SceneType dae::Scene::GetSceneType() const
{
	return m_SceneType;
}

bool dae::Scene::IsActive() const
{
	return m_IsActive;
}

void dae::Scene::SetSceneType(Scene::SceneType sceneType)
{
	m_SceneType = sceneType;
}

void dae::Scene::SetActivity(bool active)
{
	m_IsActive = active;
}

