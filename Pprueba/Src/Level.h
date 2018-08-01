#ifndef _H_LEVEL_H_
#define _H_LEVEL_H_
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

class SceneManager;
class Game;
class GameManager_c;

class Level:
	public Scene
{
public:

	Level();
	virtual ~Level();	

private:
	std::list <Entity*> entities;	

	Entity * _gameManager;

};


#endif //!_H_SCENE_H_