//#pragma once
#ifndef SCENEMANAGER_H
#define	SCENEMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	//class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void RemoveScene(const std::string& name);
		void RemoveNonActiveScenes();

		Scene* GetActiveScene() const;
		Scene* GetNextScene() const;
		void SetNextScene(const std::string& sceneToActivate);
		void SetActiveScene();

		virtual ~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;
		SceneManager& operator= (const SceneManager&) = delete;
		SceneManager& operator= (SceneManager&&) noexcept = delete;

		void Update();
		void PrepareImGuiRender();
		void Render() const;
		void FixedUpdate();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		void Start();

		void CheckForDestroyedScenes();

		std::string m_ActiveScene;
		std::string m_NextScene;
		std::vector<std::string> m_KeysToDestroy;
		std::unordered_map <std::string, std::unique_ptr <Scene>> m_pMapScenes;
	};
}
#endif // !SCENEMANAGER_H
