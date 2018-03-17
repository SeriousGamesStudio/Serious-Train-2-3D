#include "SceneManager.h"
#include "Scene.h"
#include "ObjectsFactory.h"
#include "Game.h"

#include <algorithm>

#include "btBulletCollisionCommon.h"

Scene::Scene(SceneManager * sceneManager_) :
	sceneManager(sceneManager_), isSendingMessages(false)
{
	robot = new m_Entity(this, 1, "robot");  //id a partir de 1
	entities.push_back(robot);

	robot->addComponent(new MeshRenderer_c(robot, &game_->getGraphicsManager(), "fish.mesh")); //pruebas
}

Scene::~Scene()
{
	for (m_Entity* e : entities) delete e;
}

///////////////////////////////TICK///////////////////////////////////////
void Scene::tick()
{
	if (!messages.empty())
		_msgDeliver();

	for (m_Entity* e : entities)
		e->tick();
}
///////////////////////////////TICK///////////////////////////////////////

void Scene::addListiner(MsgId id, Component* component)
{
	//Si este mensaje es nuevo, dadlo de alta junto con el componente que lo llamo
	if (!listeners.count(id)) {
		std::vector<Component*> nv;
		nv.push_back(component);
		listeners.emplace(id, nv);
	}
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

m_Entity* Scene::whoIs(EntityId id)
{
	auto entityIt = std::lower_bound(entities.begin(), entities.end(), id, [&](m_Entity* entity, EntityId id) { return entity->getId() < id; });
	if (entityIt != entities.end())
		return *entityIt;
	return nullptr;
}

m_Entity* Scene::whoIs(std::string name)
{
	for (m_Entity* entity : entities)
		if (entity->getName() == name)
			return entity;
	return nullptr;
}

std::string const Scene::getNameOf(EntityId id)
{
	return whoIs(id)->getName();
}

EntityId const Scene::getIdOf(std::string name)
{
	return whoIs(name)->getId();
}

void Scene::_msgDeliver()
{
	while (!messages.empty()) {
		isSendingMessages = true;
		Msg msg = messages.front();
		for (Component* listener : listeners[msg.id]) {
			if (msg.reciver) {
				if (msg.reciver == listener->getEntity()->getId())
					listener->listen(msg);
			}
			else
				listener->listen(msg);
		}
		messages.pop_front();
	}
	isSendingMessages = false;
}

void Scene::_dumpMessages()
{
	if (!isSendingMessages) {
		for (auto m : messagesBuffer)
			messages.push_back(m);
		messagesBuffer.clear();
	}
}

/*
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
*/
