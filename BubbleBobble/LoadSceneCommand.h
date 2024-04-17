#ifndef LOADSCENECOMMAND_H
#define LOADSCENECOMMAND_H

#include "Command.h"
#include <SceneManager.h>
#include <string>

namespace dae
{
	class LoadSceneCommand final : public Command 
	{
	public:
		LoadSceneCommand(const std::string& sceneName) :
			m_SceneName{ sceneName } 
		{};
		virtual ~LoadSceneCommand() = default;

		LoadSceneCommand(const LoadSceneCommand&) = delete;
		LoadSceneCommand(LoadSceneCommand&&) noexcept = delete;
		LoadSceneCommand& operator= (const LoadSceneCommand&) = delete;
		LoadSceneCommand& operator= (LoadSceneCommand&&) noexcept = delete;

		virtual void Execute() const override
		{
			SceneManager::GetInstance().SetActiveScene(m_SceneName);
		}
	private:
		const std::string m_SceneName;
	};
}

#endif // !LOADSCENECOMMAND_H

