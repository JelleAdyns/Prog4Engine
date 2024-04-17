#ifndef DESTROYSCENECOMMAND_H
#define DESTROYSCENECOMMAND_H

#include "Command.h"
#include <SceneManager.h>

namespace dae
{
	class DestroySceneCommand final : public Command
	{
	public:
		DestroySceneCommand(const std::string& sceneToDestroy) :
			m_SceneToDestroy{sceneToDestroy}
		{}
		virtual ~DestroySceneCommand() = default;

		DestroySceneCommand(const DestroySceneCommand&) = delete;
		DestroySceneCommand(DestroySceneCommand&&) noexcept = delete;
		DestroySceneCommand& operator= (const DestroySceneCommand&) = delete;
		DestroySceneCommand& operator= (DestroySceneCommand&&) noexcept = delete;

		virtual void Execute() const override
		{
			SceneManager::GetInstance().RemoveScene(m_SceneToDestroy);
		}

	private:
		const std::string m_SceneToDestroy;
	};
}

#endif // !DESTROYSCENECOMMAND_H