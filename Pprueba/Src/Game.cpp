#include "Game.h"
#include <ctime>
#include "Scene.h"
#include "Sound.h"
#include <functional>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#include "TrashCollector.h"

using namespace GUIndilla;
Game* Game::instance = nullptr;

void stopGame() {
	Game::getInstance()->stop();
}

Game::Game()
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

//free resources calling resources' freeing functions of each manager
Game::~Game()
{
	//¿Como son singletons hay uqe llamar a las destructuras o 
	// se eleminan automaáticamente cuando salen de scope?
	delete sceneManager;
	delete soundManager;
	delete graphicsManager;
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

	//incializar el GUI
	GUI * g = graphicsManager->getGUI();
	std::function<void()> fun = stopGame;
	g->createButton(Ogre::Vector4(0.9, 0.9, 0.1, 0.1),"bgui.button","Salir del juego",Callback(fun),POSITION_TYPE::PT_REL);
	g->createStaticImage(Ogre::Vector4(-25, -25, 50, 50), "crossAir", POSITION_TYPE::PT_ABSOLUTE, VERTICAL_ANCHOR::VA_CENTER, HORINZONTAL_ANCHOR::HA_CENTER);
	
	if (!soundManager->initialise())
		printf("SoundManager no se ha iniciado \n");
	dataManager = DataManager::getInstance();


	
	Scene * initial = new Scene();
	sceneManager->pushScene(initial);
	initial->setGameManager();
	

	run();
	return true;
}

void Game::stop()
{
	exit = true;
}

void Game::run()
{
	clock_t lastTicks = clock();
	clock_t elapsedTicks = 0;
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

