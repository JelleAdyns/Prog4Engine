#ifndef GAME_H
#define GAME_H

#include <Singleton.h>

class GameObject;

class Game final : public dae::Singleton<Game>
{
public:

	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	Game& operator= (const Game&) = delete;
	Game& operator= (Game&&) noexcept = delete;

	void StartGame() const;
private:
	void LoadLevelOne() const;
	void LoadLevelTwo() const;
	void LoadLevelThree() const;

	friend class dae::Singleton<Game>;
	Game() = default;
};

#endif // !GAME_H