#include "Game.h"
#include "Scene.h"

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
	Scene initial = Scene(&sceneManager, this);
	sceneManager.pushScene(initial);
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
	while (!exit)
	{
		inputManager->capture();
		physicsManager.stepUp();
		sceneManager.tick();
		graphicsManager.renderFrame();
	}
}
m_SceneManager const & Game::getSceneManager()		//const
{
	return sceneManager;
}

InputManager const & Game::getInputManager() const
{
	return *inputManager;
}

GraphicsManager  & Game::getGraphicsManager() 
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

