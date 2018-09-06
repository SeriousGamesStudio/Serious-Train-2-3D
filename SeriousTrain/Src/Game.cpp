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

Game::Game(): level(1)
{
}


Game * Game::getInstance()
{
	if (!instance) {
		instance = new Game();
	}
	return instance;
}

//free resources calling resources' freeing functions of each manager
Game::~Game()
{
	//¿Como son singletons hay uqe llamar a las destructuras o 
	// se eleminan automaáticamente cuando salen de scope?
	delete soundManager;
	delete sceneManager;
	delete graphicsManager;
	delete physicsManager;
	delete inputManager;
	instance = nullptr;
}

bool Game::start()
{
	//MODIFICACION DEL START PARA PODER LOCALIZAR SI HA FALLADO ALGUN MANAGER
	exit = false;

	graphicsManager = GraphicsManager::getInstance();
	if (!graphicsManager->start())
	{
		cout << "graphicsManager no inicializado" << endl;
		return false;
	}

	soundManager = SoundManager::getInstance();
	if (!soundManager->initialise())
	{
		cout << "soundManager no inicializado" << endl;
		return false;
	}

	physicsManager = PhysicsManager::getInstance();
	if (physicsManager == NULL) // culpa de borja por hacer mal el init().
	{
		cout << "physicsManager no inicializado" << endl;
		return false;
	}

	sceneManager = SceneManager::getInstance();
	if (sceneManager == NULL) // no tiene sentido que esto falle 
							  //porque no depende de ninguna libreria
	{
		cout << "sceneManager no inicializado" << endl;
		return false;
	}

	inputManager = InputManager::getInstance();
	if (inputManager == NULL)
	{
		cout << "inputManager no inicializado" << endl;
		return false;
	}
	inputManager->initialise(graphicsManager->getWindow());

	//incializar el GUI
	GUI * g = graphicsManager->getGUI();
	std::function<void()> fun = stopGame;
	g->createButton(Ogre::Vector4(0.9, 0.9, 0.1, 0.1),"bgui.button","Salir del juego",Callback(fun),POSITION_TYPE::PT_REL);
	
	
	if (!soundManager->initialise())
		printf("SoundManager no se ha iniciado \n");
	dataManager = DataManager::getInstance();


	Scene * initial = new Scene(Scene::Tipo::MENU);
	sceneManager->pushScene(initial);
	initial->setGameManager();

	/*Scene * initial = new Scene();
	sceneManager->pushScene(initial);
	initial->setGameManager();*/
	
	
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

