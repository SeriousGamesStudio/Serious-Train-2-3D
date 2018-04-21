#ifndef _H_GAME_H_
#define _H_GAME_H_

#include "SceneManager.h"
#include "InputManager.h"
#include "DataManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"

#include <iostream>


class Game
{
public:
	static Game* getInstance();
	~Game();
	
	bool start();
	bool stop();

	inline float getDeltaTime() const {
		return (float)deltaTime;
	};

private:
	Game();
	static Game* instance;
	void run();

private:
	bool exit;
	SceneManager *sceneManager;
	InputManager * inputManager;
	SoundManager * soundManager;
	GraphicsManager *graphicsManager;
	PhysicsManager *physicsManager;
	DataManager* dataManager;


	double deltaTime;/*in seconds*/

};
#endif //!_H_GAME_H_