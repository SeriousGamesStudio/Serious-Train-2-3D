#ifndef _H_SCENE_H_
#define _H_SCENE_H_

#include <list>

#include "Entity.h"

class Scene
{
public:
	Scene();
	~Scene();

	void update();
private:

	std::list<Entity*> entities;
};

#endif