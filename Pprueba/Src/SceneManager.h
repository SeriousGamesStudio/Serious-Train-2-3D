#ifndef _H_SCENEMANAGER_H_
#define _H_SCENEMANAGER_H_
#include "Scene.h"
#include <stack>

class Game;
class SceneManager
{
public:
	virtual ~SceneManager();
	static SceneManager* getInstance();
	void tick();
	inline Game* getGame() { return game; }
	Scene* currentScene();
	void pushScene(Scene* newScene);
	void popScene();
	void changeScene(Scene* newScene);
private:
	SceneManager();
	static SceneManager* instance;
	Game * game;
	std::stack<Scene*> scenes;
};
#endif //!_H_SCENEMANAGER_H_

