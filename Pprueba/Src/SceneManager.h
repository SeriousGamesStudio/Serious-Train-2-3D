#ifndef _H_SCENEMANAGER_H_
#define _H_SCENEMANAGER_H_
#include "Scene.h"
#include <stack>

class SceneManager
{
public:
	virtual ~SceneManager();
	static SceneManager* getInstance();
	void tick();
	Scene* currentScene();
	void pushScene(Scene* newScene);
	void popScene();
	void changeScene(Scene* newScene);
private:
	SceneManager();
	static SceneManager* instance;
	std::stack<Scene*> scenes;
};
#endif //!_H_SCENEMANAGER_H_

