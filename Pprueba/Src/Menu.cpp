#include "Menu.h"
#include "SceneManager.h"
#include "ObjectsFactory.h"
#include "Game.h"
#include "DataManager.h"
#include "GameManager_c.h"
#include "GraphicsManager.h"
#include "Level.h"

#include <algorithm>

#include "btBulletCollisionCommon.h"

void startGame() {
	Level* first = new Level();
	SceneManager::getInstance()->pushScene(first);
	first->setGameManager();
}
Menu::Menu() : 
	Scene(),
	 _gameManager(0)
{
	std::function<void()> fun = startGame;
	GraphicsManager::getInstance()->getGUI()->createButton(
		Ogre::Vector4(0.9, 0.9, 0.1, 0.1), "bgui.button", "Comenzar partida", 
		GUIndilla::Callback(fun), GUIndilla::POSITION_TYPE::PT_REL);

}

Menu::~Menu()
{
	
}

