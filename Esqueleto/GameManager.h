#ifndef _H_GAMEMANAGER_H_
#define _H_GAMEMANAGER_H_

#include <stack>

#include "Scene.h"

class GameManager
{
public:
	GameManager();
	~GameManager();


	void run();
private:
	void update();

private:
	std::stack<Scene*> scenes;
};
#endif // !_H_GAMEMANAGER_H_

