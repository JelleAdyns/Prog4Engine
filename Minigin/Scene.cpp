#include "Scene.h"

#include <algorithm>

using namespace dae;

void Scene::AddGameObject(std::unique_ptr<GameObject>&& object)
{
	m_pObjects.emplace_back(std::make_pair(std::move(object), false));
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_pObjects.erase(
		std::remove_if(m_pObjects.begin(), m_pObjects.end(), [&](const std::pair<std::unique_ptr<GameObject>, bool>& pObject) 
			{ return pObject.first == object; }),
		m_pObjects.end());
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
}


void dae::Scene::Start()
{
	ActivateAllObjects();

	for (auto& [pObject, active] : m_pObjects)
	{
		if (active)
			pObject->Start();
	}
}

void Scene::Update()
{
	
	ActivateAllObjects();

	for (auto& [pObject, active] : m_pObjects)
	{
		if (active)
			pObject->Update();
	}

	RemoveDead();
}

void dae::Scene::PrepareImGuiRender()
{
	for (auto& [pObject, active] : m_pObjects)
	{
		if (active)
			pObject->PrepareImGuiRender();
	}
}

void Scene::Render() const
{
	for (const auto& [pObject, active] : m_pObjects)
	{
		if (active)
			pObject->Render();
	}
}

void dae::Scene::FixedUpdate()
{
	ActivateAllObjects();

	for (const auto& [pObject, active] : m_pObjects)
	{
		if(active)
			pObject->FixedUpdate();
	}

	RemoveDead();
}

bool dae::Scene::IsDestroyed() const
{
	return m_IsDestroyed;
}

void dae::Scene::SetDestroyed()
{
	m_IsDestroyed = true;
}



void dae::Scene::RemoveDead()
{
	m_pObjects.erase(
		std::remove_if(m_pObjects.begin(), m_pObjects.end(),
			[&](const std::pair<std::unique_ptr<GameObject>, bool>& pObject) {return pObject.first->IsDead(); }
		),
		m_pObjects.cend()
	);
}

void dae::Scene::ActivateAllObjects()
{
	for (auto& [pObject, active] : m_pObjects)
	{
		active = true;
	}
}

