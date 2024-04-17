//#pragma once
#ifndef SCENEMANAGER_H
#define	SCENEMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>
#include "Singleton.h"

class Game;
namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void RemoveScene(const std::string& name);

		void SetActiveScene(const std::string& sceneToActivate);

		virtual ~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) noexcept = delete;
		SceneManager& operator= (const SceneManager&) = delete;
		SceneManager& operator= (SceneManager&&) noexcept = delete;

		void Update();
		void PrepareImGuiRender();
		void Render() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::string m_ActiveScene;
		std::unordered_map <std::string, std::shared_ptr <Scene>> m_pMapScenes;
	};
}
#endif // !SCENEMANAGER_H
