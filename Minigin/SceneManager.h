//#pragma once
#ifndef SCENEMANAGER_H
#define	SCENEMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"



namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void PrepareImGuiRender();
		void Render() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector < std::shared_ptr <Scene>> m_scenes;
	};
}
#endif // !SCENEMANAGER_H
