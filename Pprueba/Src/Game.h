#ifndef _H_GAME_H_
#define _H_GAME_H_

#include "SceneManager.h"
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

	m_SceneManager const&getSceneManager();	const//const
	InputManager const& getInputManager() const;
	GraphicsManager  &getGraphicsManager();
	DataManager const& getDataManager() const;
	PhysicsManager const & getPhysicsManager() const;


private:
	void run();

private:
	bool exit;
	m_SceneManager sceneManager;
	InputManager * inputManager;
	GraphicsManager graphicsManager;
	PhysicsManager physicsManager;
	DataManager dataManager;

};
#endif //!_H_GAME_H_