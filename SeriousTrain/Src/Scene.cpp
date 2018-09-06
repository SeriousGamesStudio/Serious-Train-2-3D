#include "Scene.h"
#include "SceneManager.h"
#include "ObjectsFactory.h"
#include "Game.h"
#include "DataManager.h"
#include "GameManager_c.h"

#include <algorithm>

#include "btBulletCollisionCommon.h"

void changeScene(Scene::Tipo tipo) {

	GraphicsManager::getInstance()->getGUI()->removeContainer(GraphicsManager::getInstance()->
		getGUI()->getOverlaytextContainer());
	Scene * initial = new Scene(tipo);
	SceneManager::getInstance()->changeScene(initial);
	initial->setGameManager();
}
Scene::Scene(Tipo tipo):
	isSendingMessages(false), _gameManager(0), t_(tipo), counter_(0), endLevel_(false)
{
	switch (t_)
	{
	case MENU:
	{
		std::string endText;

		if (Game::getInstance()->getLevel() == 3) {
			start_ = false;
			endText = "WELL DONE!! ";
			// esto es para que salga el raton el la ultima pantalla
			GraphicsManager::getInstance()->toggleMouse(true);  
		}
		else if (Game::getInstance()->getLevel() == 0) {
			start_ = false;
			endText = "GAME OVER ";
			// esto es para que salga el raton el la ultima pantalla
			GraphicsManager::getInstance()->toggleMouse(true);
		}
		else {

		start_ = true;
		endText = "Level " + std::to_string(Game::getInstance()->getLevel());
		}
		GraphicsManager::getInstance()->getGUI()->createText(Ogre::Vector4(0.3, 0.3, 0.1, 0.1),
			endText, GUIndilla::POSITION_TYPE::PT_REL, 25);
	}
	break;
	case LEVEL:
	{
		start_ = false;
		numEnemigos_ = 0; 
		std::string sceneDataPath;
		if (Game::getInstance()->getLevel() == 1) 
		{
			sceneDataPath = "..\\Data\\Levels\\Escena_1.xml";//Esto queda por ver cómo darle valor y tal leyendo de fichero
		}
		else 
			sceneDataPath = "..\\Data\\Levels\\Escena_2.xml";
		SceneData* sceneData = DataManager::getInstance()->loadScene(sceneDataPath);
		for (auto entityData : *sceneData)
		{
			Entity* newEntity = ObjectsFactory::getInstance()->create(*entityData, this);
			entities.push_back(newEntity);
		}
		delete sceneData;

		

		GraphicsManager::getInstance()->getGUI()->createStaticImage(Ogre::Vector4(-25, -25, 50, 50), "crossAir", GUIndilla::POSITION_TYPE::PT_ABSOLUTE,
			GUIndilla::VERTICAL_ANCHOR::VA_CENTER, GUIndilla::HORINZONTAL_ANCHOR::HA_CENTER);

		for (auto i : entities) {
			if (i->getComponent(ComponentType::ENEMYBEHAVIOUR))
				numEnemigos_++;
		}
		
			
		enRes_ =  std::to_string(numEnemigos_) + " enemies ";
		GraphicsManager::getInstance()->getGUI()->createText(Ogre::Vector4(0.05, 0.05, 0.9, 0.9),
			enRes_, GUIndilla::POSITION_TYPE::PT_REL, 25);

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
	entities.clear();

	for (auto e : messages) {
		delete e;
	}

	for (auto e : messagesBuffer) {
		delete e;
	}

	for (auto e : graveland) {
		delete e;
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

	if (start_ && counter_ >= 50) {
		start_ = false;
		changeScene(Scene::Tipo::LEVEL);
	}
	else if (start_ && counter_ < 50){

		counter_++;
	}
	if (endLevel_ && counter_ >= 50) {
		endLevel_ = false;
		GraphicsManager::getInstance()->getGUI()->removeContainer(GraphicsManager::getInstance()->
			getGUI()->getOverlaytrainhpContainer());
		changeScene(Scene::Tipo::MENU);
	}
	else if (endLevel_ && counter_ < 50) {
		counter_++;
		for (Entity* e : entities) {
			e->destroy();
		}
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

void Scene::restaEnemigo()
{
	
	numEnemigos_--;
	enRes_ = std::to_string(numEnemigos_) + " enemies ";
	GraphicsManager::getInstance()->getGUI()->removeContainer(GraphicsManager::getInstance()->
		getGUI()->getOverlaytextContainer());
	GraphicsManager::getInstance()->getGUI()->createText(Ogre::Vector4(0.05, 0.05, 0.9, 0.9),
		enRes_, GUIndilla::POSITION_TYPE::PT_REL, 25);

	if (numEnemigos_ == 0) {
		//cambiar menu ppal
		Game::getInstance()->levelUp();
		endLevel_ = true;
		
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
			else if (!endLevel_) {

				listener->listen(msg); // aqui falla 
			}
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
