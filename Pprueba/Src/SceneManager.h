#ifndef _H_SCENEMANAGER_H_
#define _H_SCENEMANAGER_H_
#include "Scene.h"
#include <stack>

class Game;
class m_SceneManager
{
public:
	m_SceneManager(Game* game_);
	virtual ~m_SceneManager();

	void tick();

	Scene& currentScene();
	void pushScene(Scene& newScene);
	void popScene();
	void changeScene(Scene& newScene);
private:
	Game * game;
	std::stack<Scene> scenes;
};
#endif //!_H_SCENEMANAGER_H_

