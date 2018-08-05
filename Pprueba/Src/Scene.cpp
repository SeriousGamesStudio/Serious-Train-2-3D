#include "Scene.h"
#include "SceneManager.h"
#include "ObjectsFactory.h"
#include "Game.h"
#include "DataManager.h"
#include "GameManager_c.h"

#include <algorithm>

#include "btBulletCollisionCommon.h"
void startGame() {
	Scene * initial = new Scene(Scene::Tipo::LEVEL);
	//Sabemos que esto es una chapuza pero no podiamos perder mas tiempo en esto ya que 
	//fran era el encargado de hacerlo y sin solucionarlo asi ni el sabia como
	InputManager::getInstance()->stop();
	SceneManager::getInstance()->pushScene(initial);
	initial->setGameManager();
}
Scene::Scene(Tipo tipo) :
	isSendingMessages(false), _gameManager(0), t(tipo)
{
	
	switch (t)
	{
	case MENU:
	{
		std::function<void()> fun = startGame;
			GraphicsManager::getInstance()->getGUI()->createButton(
				Ogre::Vector4(0.9, 0.9, 0.1, 0.1), "bgui.button", "Comenzar partida", 
				GUIndilla::Callback(fun), GUIndilla::POSITION_TYPE::PT_REL);
	}
		break;
	case LEVEL:
	{
		std::string sceneDataPath = "..\\Data\\Levels\\exampleAuto.xml";//Esto queda por ver cómo darle valor y tal leyendo de fichero
		SceneData* sceneData = DataManager::getInstance()->loadScene(sceneDataPath);
		for (auto entityData : *sceneData)
		{
			Entity* newEntity = ObjectsFactory::getInstance()->create(*entityData, this);
			entities.push_back(newEntity);
		}
		delete sceneData;

		GraphicsManager::getInstance()->getGUI()->createStaticImage(Ogre::Vector4(-25, -25, 50, 50), "crossAir",
			GUIndilla::POSITION_TYPE::PT_ABSOLUTE, GUIndilla::VERTICAL_ANCHOR::VA_CENTER, GUIndilla::HORINZONTAL_ANCHOR::HA_CENTER);
		}
			break;
		default:
			break;
	}

}

Scene::~Scene()
{
	delete _gameManager;
	for (Entity* e : entities) {
		delete e;
		e = nullptr;
	}
}

///////////////////////////////TICK///////////////////////////////////////
void Scene::tick()
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

void Scene::reciveMsg(Msg_Base* newMsg)
{
	if (isSendingMessages)
		messagesBuffer.push_back(newMsg);
	else
		messages.push_back(newMsg);
}

Entity* Scene::whoIs(EntityId id)
{
	auto entityIt = std::lower_bound(entities.begin(), entities.end(), id,
		[&](Entity* entity, EntityId id) { return entity->getId() < id; }
	);
	if (entityIt != entities.end())
		return *entityIt;
	return nullptr;
}

Entity* Scene::whoIs(std::string name)
{
	for (Entity* entity : entities)
		if (entity->getName() == name)
			return entity;
	return nullptr;
}

void Scene::setGameManager()
{
	if (!_gameManager) {
		_gameManager = new Entity(this, 0, "GameManager");
		_gameManager->addComponent(new GameManager_c());
		_gameManager->init();
	}

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

void Scene::_dumpMessages()
{
	if (!isSendingMessages) {
		for (auto m : messagesBuffer)
			messages.push_back(m);
		messagesBuffer.clear();
	}
}

void Scene::destroyEntity(Entity* entity)
{
	entities.remove(entity);
	delete entity;
}