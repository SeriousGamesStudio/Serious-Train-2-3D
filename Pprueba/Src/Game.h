#ifndef _H_GAME_H_
#define _H_GAME_H_

#include "GameManager.h"
#include "InputManager.h"
#include "DataManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"

#include <iostream>


class Game
{
public:
	Game();
	~Game();

	bool start();
	bool stop();
	void run();


	GameManager getGameManager() const;
	InputManager getInputManager() const;
	GraphicsManager getGraphicsManager() const;
	DataManager getDataManager() const;
	PhysicsManager getPhysicsManager() const;


private:
	GameManager gameManager;
	InputManager  inputManager;
	GraphicsManager graphicsManager;
	PhysicsManager physicsManager;
	DataManager dataManager;

};
#endif //!_H_GAME_H_