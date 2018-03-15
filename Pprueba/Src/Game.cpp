#include "Game.h"

Game::Game() :
	sceneManager(this), inputManager(this),
	dataManager(this), graphicsManager(this),
	physicsManager(this)
{
}

Game::~Game()
{
	//liberar recursos
}

bool Game::start()
{
	exit = false;
	graphicsManager.start();
	run();
	return true;
}

bool Game::stop()
{
	exit = true;
	return true;
}

void Game::run()
{
	while (!exit)
	{
		inputManager.update();
		physicsManager.stepUp();
		sceneManager.tick();
		graphicsManager.run();
	}
}
m_SceneManager const & Game::getSceneManager() const
{
	return sceneManager;
}

InputManager const & Game::getInputManager() const
{
	return inputManager;
}

GraphicsManager const & Game::getGraphicsManager() const
{
	return graphicsManager;
}

DataManager const & Game::getDataManager() const
{
	return dataManager;
}

PhysicsManager const & Game::getPhysicsManager() const
{
	return physicsManager;
}

