#include "Game.h"
#include <ctime>
#include "Scene.h"

Game::Game() :
	sceneManager(this),
	dataManager(this), graphicsManager(this),
	physicsManager(this)
{
}


//TODO: destructor not implemented
Game::~Game()
{
	//free resources calling resources' freeing functions of each manager
}

bool Game::start()
{

	exit = false;
	graphicsManager.start();
	inputManager = InputManager::getSingletonPtr();
	inputManager->initialise(graphicsManager.getWindow());
	Scene initial = Scene(&sceneManager, this);
	sceneManager.pushScene(initial);


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
	clock_t lastTicks = clock();
	clock_t elapsedTicks;
	double deltaTime;/*in seconds*/
	while (!exit)
	{
		//getting the time passed since last frame
		elapsedTicks = clock() - lastTicks;
		deltaTime = ((float)elapsedTicks) / CLOCKS_PER_SEC;
		{/////////////MANAGERS UPDATE/////////////
			inputManager->capture();
			physicsManager.stepUp(deltaTime);
			sceneManager.tick();
			graphicsManager.renderFrame();
		}/////////////////////////////////////////

		lastTicks = clock();
	}
}
SceneManager const & Game::getSceneManager()		//const
{
	return sceneManager;
}

InputManager * Game::getInputManager()
{
	return inputManager;
}

GraphicsManager  & Game::getGraphicsManager()
{
	return graphicsManager;
}

DataManager const & Game::getDataManager() const
{
	return dataManager;
}

PhysicsManager & Game::getPhysicsManager() {
	return physicsManager;
}

