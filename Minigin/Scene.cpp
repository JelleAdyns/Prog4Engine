#include "Scene.h"

#include <algorithm>
namespace dae
{

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


	void Scene::Start()
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

	void Scene::PrepareImGuiRender()
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

	void Scene::FixedUpdate()
	{
		ActivateAllObjects();

		for (const auto& [pObject, active] : m_pObjects)
		{
			if (active)
				pObject->FixedUpdate();
		}

		RemoveDead();
	}

	bool Scene::IsDestroyed() const
	{
		return m_IsDestroyed;
	}

	void Scene::SetDestroyed()
	{
		m_IsDestroyed = true;
	}



	void Scene::RemoveDead()
	{
		m_pObjects.erase(
			std::remove_if(m_pObjects.begin(), m_pObjects.end(),
				[&](const std::pair<std::unique_ptr<GameObject>, bool>& pObject) {return pObject.first->IsDead(); }
			),
			m_pObjects.cend()
		);
	}

	void Scene::ActivateAllObjects()
	{
		for (auto& [pObject, active] : m_pObjects)
		{
			active = true;
		}
	}
}

