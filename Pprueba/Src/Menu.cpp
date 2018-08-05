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
	/*Level* first = new Level();
	SceneManager::getInstance()->pushScene(first);
	first->setGameManager();*/
}
Menu::Menu() : 
	 _gameManager(0)
{
	/*std::function<void()> fun = startGame;
	GraphicsManager::getInstance()->getGUI()->createButton(
		Ogre::Vector4(0.9, 0.9, 0.1, 0.1), "bgui.button", "Comenzar partida", 
		GUIndilla::Callback(fun), GUIndilla::POSITION_TYPE::PT_REL);*/

}

Menu::~Menu()
{
	delete _gameManager;
	for (Entity* e : entities) {
		delete e;
		e = nullptr;
	}
}
///////////////////////////////TICK///////////////////////////////////////
void Menu::tick()
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

void Menu::addListiner(MsgId id, Component* component)
{
	//Si este mensaje es nuevo, dadlo de alta junto con el componente que lo llamo
	if (!listeners.count(id)) {
		std::vector<Component*> nv;
		nv.push_back(component);
		listeners.emplace(id, nv);
	}
	//Si ya existe, simplemente a�ade el componente a la lista
	else
		listeners[id].push_back(component);
}

void Menu::removeListener(MsgId id, Component* component)
{
	//Si hay alguien suscrito a este mensaje
	if (listeners.count(id)) {
		auto it = listeners[id].begin();
		auto end = listeners[id].end();
		//encuentra el componente que se quieres desuscribir
		while (component != *it && it != end) it++;
		//si se ha encontrado, elim�nalo
		if (it != end) listeners[id].erase(it);
	}
}

void Menu::reciveMsg(Msg_Base* newMsg)
{
	if (isSendingMessages)
		messagesBuffer.push_back(newMsg);
	else
		messages.push_back(newMsg);
}

Entity* Menu::whoIs(EntityId id)
{
	auto entityIt = std::lower_bound(entities.begin(), entities.end(), id,
		[&](Entity* entity, EntityId id) { return entity->getId() < id; }
	);
	if (entityIt != entities.end())
		return *entityIt;
	return nullptr;
}

Entity* Menu::whoIs(std::string name)
{
	for (Entity* entity : entities)
		if (entity->getName() == name)
			return entity;
	return nullptr;
}

void Menu::setGameManager()
{
	if (!_gameManager) {
		_gameManager = new Entity(this, 0, "GameManager");
		_gameManager->addComponent(new GameManager_c());
		_gameManager->init();
	}

}

std::string const Menu::getNameOf(EntityId id)
{
	return whoIs(id)->getName();
}

EntityId const Menu::getIdOf(std::string name)
{
	return whoIs(name)->getId();
}

void Menu::_msgDeliver()
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

void Menu::_dumpMessages()
{
	if (!isSendingMessages) {
		for (auto m : messagesBuffer)
			messages.push_back(m);
		messagesBuffer.clear();
	}
}

void Menu::destroyEntity(Entity* entity)
{
	entities.remove(entity);
	delete entity;
}

