#ifndef GAME_H
#define GAME_H

class GameObject;

class Game
{
public:

	~Game() {}

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	Game& operator= (const Game&) = delete;
	Game& operator= (Game&&) noexcept = delete;

private:
};

#endif // !GAME_H