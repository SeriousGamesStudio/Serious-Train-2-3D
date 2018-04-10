#include "Game.h"
#include <ctime>
#include "Scene.h"
#include "Sound.h"
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#include "TrashCollector.h"

Game* Game::instance = nullptr;

Game::Game() :
	dataManager(this)
{
}


Game * Game::getInstance()
{
	if (!instance) {
		instance = new Game();
		instance->start();
	}
	return instance;
}

//TODO: destructor not implemented
//free resources calling resources' freeing functions of each manager
Game::~Game()
{
	//¿Como son singletons hay uqe llamar a las destructuras o 
	// se eleminan automaáticamente cuando salen de scope?
	delete soundManager;
	delete graphicsManager;
	delete sceneManager;
	delete physicsManager;
	delete inputManager;
	instance = nullptr;
}

bool Game::start()
{
	exit = false;

	graphicsManager = GraphicsManager::getInstance();

	soundManager = SoundManager::getInstance();

	physicsManager = PhysicsManager::getInstance();

	sceneManager = SceneManager::getInstance();

	inputManager = InputManager::getInstance();
	inputManager->initialise(graphicsManager->getWindow());

#ifdef _DEBUG
	if(!soundManager->initialise())
		printf("SoundManager no se ha iniciado \n");
#endif // _DEBUG
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
	sceneManager->pushScene(new Scene());
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

