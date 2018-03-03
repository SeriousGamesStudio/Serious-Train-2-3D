#ifndef _H_GAME_H_
#define _H_GAME_H_
#pragma once

#include "GameManager.h"
#include "InputManager.h"
#include "DataManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"


class Game
{
public:
	Game();
	~Game();

	bool start();
	bool stop();

	GameManager getGameManager() const;
	InputManager getInputManager() const;
	GraphicsManager getGraphicsManager() const;
	DataManager getDataManager() const;
	PhysicsManager getPhysicsManager() const;


private:
	void run();

private:
	GameManager gameManager;
	InputManager  inputManager;
	GraphicsManager graphicsManager;
	PhysicsManager physicsManager;
	DataManager dataManager;

};
#endif //!_H_GAME_H_