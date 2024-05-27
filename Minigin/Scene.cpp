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
	for (auto& pair : m_pObjects)
	{
		pair.second = true;
	}
	for (auto& pair : m_pObjects)
	{
		if(pair.second)
		pair.first->Start();
	}
}

void Scene::Update()
{
	for (auto& pair : m_pObjects)
	{
		pair.second = true;
	}

	for(auto& pair : m_pObjects)
	{
		if(pair.second)
		pair.first->Update();
	}

	RemoveDead();
}

void dae::Scene::PrepareImGuiRender()
{
	for (auto& pair : m_pObjects)
	{
		if(pair.second)
		pair.first->PrepareImGuiRender();
	}
}

void Scene::Render() const
{
	for (const auto& pair : m_pObjects)
	{
		if(pair.second)
		pair.first->Render();
	}
}

void dae::Scene::FixedUpdate()
{
	for (const auto& pair : m_pObjects)
	{
		if(pair.second)
		pair.first->FixedUpdate();
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

