#include "Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::update()
{
	for (Entity* entity : entities)
		entity->update();
}
