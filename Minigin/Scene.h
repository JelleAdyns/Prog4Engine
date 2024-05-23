//#pragma once
#ifndef SCENE_H
#define	SCENE_H

#include "GameObject.h"
//#include "SceneManager.h"

namespace dae
{
	class SceneManager;
	class Scene final
	{
		//friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		
		void AddGameObject(std::unique_ptr<GameObject>&& object);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Start();
		void Update();
		void PrepareImGuiRender();
		void Render() const;
		void FixedUpdate();

		bool IsDestroyed() const;
		void SetDestroyed();

		Scene() = default;
		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		
		bool m_IsDestroyed{ false };
		std::vector < std::unique_ptr<GameObject>> m_pObjects{};
	};

}
#endif // !SCENE_H
