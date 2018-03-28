#include "Game.h"
#include <ctime>
#include "Scene.h"
#include "Sound.h"
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#include "TrashCollector.h"

Game::Game() :
	dataManager(this)
{
}


//TODO: destructor not implemented
Game::~Game()
{
	delete soundManager;
	delete graphicsManager;
	delete sceneManager;
	delete physicsManager;
	delete inputManager;
	//free resources calling resources' freeing functions of each manager
}

bool Game::start()
{

	exit = false;

	graphicsManager = new GraphicsManager(this);
	graphicsManager->start();

	soundManager = new SoundManager();

	physicsManager = new PhysicsManager(this);

	sceneManager = new SceneManager(this);

	inputManager = InputManager::getSingletonPtr();
	inputManager->initialise(graphicsManager->getWindow());

	if(!soundManager->initialise())
		printf("SoundManager no se ha iniciado \n");


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
	clock_t elapsedTicks = 0;
	double deltaTime;/*in seconds*/
	sceneManager->pushScene(new Scene(sceneManager, this));
	while (!exit)
	{
		//getting the time passed since last frame
		deltaTime = ((double)elapsedTicks) / 1000.f/*CLOCKS_PER_SEC*/;
		lastTicks = clock();
		{/////////////MANAGERS UPDATE/////////////
			inputManager->capture();
			physicsManager->stepUp(deltaTime);
			sceneManager->tick();
			graphicsManager->renderFrame();
		}/////////////////////////////////////////
		elapsedTicks = clock() - lastTicks;
	}
}
SceneManager const & Game::getSceneManager()		//const
{
	return *sceneManager;
}

InputManager * Game::getInputManager()
{
	return inputManager;
}

SoundManager * Game::getSoundManager()
{
	return soundManager;
}

GraphicsManager  & Game::getGraphicsManager()
{
	return *graphicsManager;
}

DataManager const & Game::getDataManager() const
{
	return dataManager;
}

PhysicsManager & Game::getPhysicsManager() {
	return *physicsManager;
}

