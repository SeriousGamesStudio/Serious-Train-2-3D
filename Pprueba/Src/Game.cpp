#include "Game.h"

Game::Game() :
	sceneManager(this),
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
	inputManager = InputManager::getSingletonPtr();
	inputManager->initialise(graphicsManager.getWindow());

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
	while (!graphicsManager.getWindow()->isClosed())
	{
		inputManager->capture();
		physicsManager.stepUp();
		sceneManager.tick();
		graphicsManager.run();
	}
}
SceneManager const & Game::getSceneManager() const
{
	return sceneManager;
}

InputManager const & Game::getInputManager() const
{
	return *inputManager;
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

