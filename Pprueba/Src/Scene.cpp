#include "Scene.h"
#include <algorithm>


Scene::Scene(GameManager* gameManager_) :
	gameManager(gameManager_), isSendingMessages(false)
{

}

Scene::~Scene()
{
	for (Entity* e : entities) delete e;
}
///////////////////////////////TICK///////////////////////////////////////
void Scene::tick()
{
	if (!messages.empty())
		_msgDeliver();

	for (Entity* e : entities)
		e->tick();
}
///////////////////////////////TICK///////////////////////////////////////
void Scene::addListiner(MsgId id, Component* component)
{
	//Si este mensaje es nuevo, dadlo de alta junto con el componente que lo llamo
	if (!listeners.count(id))
		listeners.emplace(id, component);
	//Si ya existe, simplemente añade el componente a la lista
	else
		listeners[id].push_back(component);
}

void Scene::removeListener(MsgId id, Component* component)
{
	//Si hay alguien suscrito a este mensaje
	if (listeners.count(id)) {
		auto it = listeners[id].begin();
		auto end = listeners[id].end();
		//encuentra el componente que se quieres desuscribir
		while (component != *it && it != end) it++;
		//si se ha encontrado, elimínalo
		if (it != end) listeners[id].erase(it);
	}
}

void Scene::reciveMsg(Msg& newMsg)
{
	if (isSendingMessages)
		messagesBuffer.push_back(newMsg);
	else
		messages.push_back(newMsg);
}

Entity* Scene::whoIs(EntityId id)
{
	auto entityIt = std::lower_bound(entities.begin(), entities.end(), id, [&](Entity* entity, EntityId id) { return entity->getId() < id; });
	if (entityIt != entities.end())
		return *entityIt;
	return nullptr;
}

Entity* Scene::whoIs(std::string name)
{
	for (Entity* entity : entities)
		if (entities.getName() == name)
			return entity;
	return nullptr;
}

std::string const Scene::getNameOf(EntityId id)
{
	whoIs(id)->getName();
}

EntityId const Scene::getIdOf(std::string name)
{
	whoIs(name)->getId();
}

void Scene::_msgDeliver()
{

}

void Scene::_dumpMessages()
{

}

template<typename T>
Component * Scene::getComponentOf(EntityId id)
{
	return nullptr;
}
template<typename T>
Component * Scene::getComponentOf(std::string name)
{
	return nullptr;
}
