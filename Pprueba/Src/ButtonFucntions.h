#ifndef _H_BUTTONFUNCTIONS_H_
#define _H_BUTTONFUNCTIONS_H_
#include "Game.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
inline void stopGame() {
	Game::getInstance()->stop();
}

inline void startGame() {
	GraphicsManager::getInstance()->getGUI()->ClearGUI();
	Scene * initial = new Scene();
	SceneManager::getInstance()->pushScene(initial);
	initial->setGameManager();
}

inline void StartMenu() {
	using namespace GUIndilla;
	GUIndilla::GUI *g = GraphicsManager::getInstance()->getGUI();
	std::function<void()> fun = stopGame;
	g->createButton(Ogre::Vector4(-100, 100, 150, 50), "bgui.button", "Salir del Juego", GUIndilla::Callback(fun), POSITION_TYPE::PT_ABSOLUTE, VERTICAL_ANCHOR::VA_CENTER, HORINZONTAL_ANCHOR::HA_CENTER);
	fun = startGame;
	g->createButton(Ogre::Vector4(-100, 0, 150, 50), "bgui.button", "Comenzar ", GUIndilla::Callback(fun), POSITION_TYPE::PT_ABSOLUTE, VERTICAL_ANCHOR::VA_CENTER, HORINZONTAL_ANCHOR::HA_CENTER);
	g->createStaticText(Ogre::Vector4(-100, -200, 150, 50), "Serious Train 2 3D", POSITION_TYPE::PT_ABSOLUTE, VERTICAL_ANCHOR::VA_CENTER, HORINZONTAL_ANCHOR::HA_CENTER);

}

#endif //!_H_M_ENTITY_H_