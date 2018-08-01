#ifndef _H_MENU_H_
#define _H_MENU_H_
#pragma once

#include <list>
#include <deque>
#include <vector>
#include <unordered_map>

#include "Scene.h"
#include "Component.h"
#include "Notifications.h"
#include "Entity.h"
#include "Components.h"
#include "MeshRenderer_c.h"

class SceneManager;
class Game;
class GameManager_c;


class Menu:
	public Scene
{
public:

	Menu();
	virtual ~Menu();

private:

	Entity * _gameManager;
	
};


#endif //!_H_MENU_H_
