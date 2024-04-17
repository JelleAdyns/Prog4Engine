//#pragma once
#ifndef SCENE_H
#define	SCENE_H

#include "GameObject.h"
#include "SceneManager.h"

namespace dae
{
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		enum class SceneType
		{
			TitleScreen,
			Menu,
			Level,
			HighScore
		};
		void AddGameObject(std::unique_ptr<GameObject>&& object);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void PrepareImGuiRender();
		void Render() const;

		Scene::SceneType GetSceneType() const;
		bool IsActive() const;
		void SetSceneType(Scene::SceneType sceneType);
		void SetActivity(bool active);

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		Scene();

		bool m_IsActive{ false };
		SceneType m_SceneType{ SceneType::TitleScreen };
		std::vector < std::unique_ptr<GameObject>> m_pObjects{};

		static unsigned int m_IdCounter; 
	};

}
#endif // !SCENE_H
