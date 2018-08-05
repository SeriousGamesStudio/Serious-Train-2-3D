#include "Level.h"
#include "SceneManager.h"
#include "ObjectsFactory.h"
#include "Game.h"
#include "DataManager.h"
#include "GameManager_c.h"

#include <algorithm>

#include "btBulletCollisionCommon.h"
Level::Level() :
	_gameManager(0)
{
	
}

Level::~Level()
{
	delete _gameManager;
	for (Entity* e : entities) {
		delete e;
		e = nullptr;
	}
}
///////////////////////////////TICK///////////////////////////////////////
void Level::tick()
{
	if (!graveland.empty()) {
		for (auto e : graveland)
			destroyEntity(e);
		graveland.clear();
	}
	if (!messages.empty())
		_msgDeliver();


	for (Entity* e : entities) {
		e->tick();
		if (!e->isAlive())
			graveland.push_back(e);
	}
}
///////////////////////////////TICK///////////////////////////////////////

void Level::addListiner(MsgId id, Component* component)
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

void Level::removeListener(MsgId id, Component* component)
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

void Level::reciveMsg(Msg_Base* newMsg)
{
	if (isSendingMessages)
		messagesBuffer.push_back(newMsg);
	else
		messages.push_back(newMsg);
}

Entity* Level::whoIs(EntityId id)
{
	auto entityIt = std::lower_bound(entities.begin(), entities.end(), id,
		[&](Entity* entity, EntityId id) { return entity->getId() < id; }
	);
	if (entityIt != entities.end())
		return *entityIt;
	return nullptr;
}

Entity* Level::whoIs(std::string name)
{
	for (Entity* entity : entities)
		if (entity->getName() == name)
			return entity;
	return nullptr;
}

void Level::setGameManager()
{
	if (!_gameManager) {
		_gameManager = new Entity(this, 0, "GameManager");
		_gameManager->addComponent(new GameManager_c());
		_gameManager->init();
	}

}

std::string const Level::getNameOf(EntityId id)
{
	return whoIs(id)->getName();
}

EntityId const Level::getIdOf(std::string name)
{
	return whoIs(name)->getId();
}

void Level::_msgDeliver()
{
	while (!messages.empty()) {
		isSendingMessages = true;
		Msg_Base* msg = messages.front();
		for (Component* listener : listeners[msg->id]) {
			if (msg->reciver != Msg_Base::broadcast) {
				if (msg->reciver == listener->getEntity()->getId())
					listener->listen(msg);
			}
			else
				listener->listen(msg);
		}
		delete msg;
		messages.pop_front();
	}
	isSendingMessages = false;
}

void Level::_dumpMessages()
{
	if (!isSendingMessages) {
		for (auto m : messagesBuffer)
			messages.push_back(m);
		messagesBuffer.clear();
	}
}

void Level::destroyEntity(Entity* entity)
{
	entities.remove(entity);
	delete entity;
}

