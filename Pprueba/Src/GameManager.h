#ifndef _H_GAMEMANAGER_H_
#define _H_GAMEMANAGER_H_
#include "Scene.h"
#include <stack>

class Game;
class GameManager
{
public:
	GameManager(Game* game_);
	virtual ~GameManager();

	void tick();

	Scene& currentScene();
	void pushScene(Scene& newScene);
	void popScene();
	void changeScene(Scene& newScene);
private:
	Game * game;
	std::stack<Scene> scenes;
};
#endif //!_H_GAMEMANAGER_H_

