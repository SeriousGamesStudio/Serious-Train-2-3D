#include "SceneManager.h"
#include "Scene.h"
#include "ObjectsFactory.h"
#include "Game.h"

#include <algorithm>

#include "btBulletCollisionCommon.h"
#include "Components.h"
Scene::Scene():
	isSendingMessages(false), _gameManager(0)
{
	//OBJETO PESCADO DE PRUEBA
	Entity* robot = new Entity(this, 1, "robot");  //id a partir de 1
	entities.push_back(robot);

	robot->addComponent(new MeshRenderer_c("fish.mesh")); //pruebas
														  //robot->addComponent(new PlayerController_c()); //pruebas
	robot->addComponent(new Animation_c());
	robot->init();

	//vagon
	


	//CAMARA DE PRUEBA
	Entity* player = new Entity(this, 2, "Player");
	entities.push_back(player);

	player->addComponent(new Camera_c());//pruebas camara
	player->addComponent(new CameraController_c()); // pruebas
	{//Add rigidBody
		btCollisionShape* fallShape = new btSphereShape(1);
		btDefaultMotionState* fallMotionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0)));
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		fallShape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
		player->addComponent(new RigidBody_c(fallRigidBodyCI));
	}

	// PLANO PRUEBAS
	Entity* plane = new Entity(this, 3, "Plano");
	entities.push_back(plane);

	plane->addComponent(new PlaneRenderer_c("plane", "nm_bk.png"));
	// el ultimo parametro es la imagen que hace de textura del plano por si quieres cambiarla
	player->addComponent(new Walker_c());
	player->addComponent(new PlayerController_c());
	player->init();


	//ANIMACION PRUEBAS






	{//Add rigidBody
		btCollisionShape* fallShape = new btSphereShape(1);
		btDefaultMotionState* fallMotionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 30, 0)));
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		fallShape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
		robot->addComponent(new RigidBody_c(fallRigidBodyCI));
	}
	//////////////////////////////////////////////////////////////////////////
	Entity* ground = new Entity(this, 4, "ground");
	ground->addComponent(new MeshRenderer_c("WoodPallet.mesh")); //pruebas
	entities.push_back(ground);
	{
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
		btDefaultMotionState* groundMotionState =
			new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -25, 0)));
		btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
		ground->addComponent(new RigidBody_c(groundRigidBodyCI));
	}
	ground->init();
	
	//vagon
	Entity* vagon = new Entity(this, 5, "vagon");
	vagon->addComponent(new MeshRenderer_c("Vagon.mesh"));
	vagon->init();
}

Scene::~Scene()
{
	for (Entity* e : entities) delete e;
	delete _gameManager;
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
			if (msg->reciver) {
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