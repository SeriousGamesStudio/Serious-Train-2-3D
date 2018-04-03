#include "Game.h"
#include <ctime>
#include "Scene.h"
#include "Sound.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
using namespace BetaGUI;
Game::Game() :
	sceneManager(this),
	dataManager(this), graphicsManager(this),
	physicsManager(this)
{
	soundManager = new SoundManager();
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
	if(!soundManager->initialise())
		printf("SoundManager no se ha iniciado \n");
	Scene initial = Scene(&sceneManager, this);
	sceneManager.pushScene(initial);
	mGUI = new GUI("Caption",13);
	BetaGUI::Window* window = mGUI->createWindow(Ogre::Vector4(100, 100, 300, 100), "bgui.window", BetaGUI::WT_NONE, "Magical Doubler");
	//window->createStaticText(Ogre::Vector4(4, 22, 198, 40), "Type in a number and I'll double it!");
	BetaGUI::Button* mDoubleIt = window->createButton(Ogre::Vector4(108, 50, 104, 24), "bgui.button", "Go on then!", BetaGUI::Callback());
	mGUI->createWindow(Ogre::Vector4(500, 100, 300, 100), "bgui.window", BetaGUI::WT_NONE, "verdad")->createStaticText(Ogre::Vector4(4, 22, 198, 40), "la Madre de manu la chupa");
	mGUI->createMousePointer(Ogre::Vector2(32, 32), "bgui.pointer");
	inputManager->addMouseListener(mGUI, "GUI");
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
	while (!exit)
	{
		//getting the time passed since last frame
		deltaTime = ((double)elapsedTicks) / 1000.f/*CLOCKS_PER_SEC*/;
		lastTicks = clock();
		{/////////////MANAGERS UPDATE/////////////
			inputManager->capture();
			physicsManager.stepUp(deltaTime);
			sceneManager.tick();
			graphicsManager.renderFrame();
		}/////////////////////////////////////////
		elapsedTicks = clock() - lastTicks;
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

SoundManager * Game::getSoundManager()
{
	return soundManager;
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

