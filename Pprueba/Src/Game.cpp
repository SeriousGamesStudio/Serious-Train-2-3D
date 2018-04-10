#include "Game.h"
#include <ctime>
#include "Scene.h"
#include "Sound.h"
#include <functional>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#include "TrashCollector.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
using namespace GUIndilla;
Game* Game::instance = nullptr;

void printNothingImportant() {
	printf("lmao this nigga \n");
}

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
	delete mGUI;
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

	if(!soundManager->initialise())
		printf("SoundManager no se ha iniciado \n");
	Scene * initial = new Scene();
	sceneManager->pushScene(initial);
	mGUI = new GUI("Caption",13);
	Window* window = mGUI->createWindow(Ogre::Vector4(100, 100, 300, 100), "bgui.window", WT_NONE, "Magical");
	//window->createStaticText(Ogre::Vector4(4, 22, 198, 40), "Type in a number and I'll double it!");
	std::function<void()> printshit = printNothingImportant;
	Button* mDoubleIt = window->createButton(Ogre::Vector4(108, 50, 104, 24), "bgui.button", "Go on then!", Callback(printshit));
	mGUI->createWindow(Ogre::Vector4(500, 100, 300, 100), "bgui.window",WT_MOVE, "verdad")->createButton(Ogre::Vector4(4, 22, 198, 40), "bgui.button","la Madre de manu la chupa",Callback());
	mGUI->createMousePointer(Ogre::Vector2(32, 32), "bgui.pointer");
	
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