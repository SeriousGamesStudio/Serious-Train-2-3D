#include "Entity.h"
#include "Scene.h"
#include "Component.h"
#include <iostream>

Entity::Entity(Scene* scene_, EntityId id_, std::string name_) :
	_scene(scene_), _id(id_), _name(name_), _alive(true)
{

}

Entity::~Entity()
{
#ifdef _DEBUG
	std::cout << "*********************\nEliminando " << getName() << "...\nComenzamos a eliminar componentes:\n";
#endif // _DEBUG
	//Iterate through all components and delete them
	for (Component* c : components)
	{
#ifdef _DEBUG
		std::cout << "    Eliminando "<< getComponentString[c->getComponentType()]<<". ";
#endif // _DEBUG
		c->disable();
		delete c;
		c = nullptr;
#ifdef _DEBUG
		std::cout << "Eliminado!\n";
#endif // _DEBUG
	}
	//make sure every component is deleted
	components.clear();
#ifdef _DEBUG
	std::cout << "    Limpiando cola de mensajes...";
#endif // _DEBUG
	while (!messages.empty()) {
		delete messages.front();
		messages.pop();
	}
#ifdef _DEBUG
	std::cout << "Cola Limpia!\n" << getName() << " Eliminado!\n*********************\n";
#endif // _DEBUG
}

void Entity::init()
{
	//get ride of extra space in components vector
	components.shrink_to_fit();

	//call the start method of every component
	for (Component* c : components)	c->start();
}

void Entity::tick()
{
	sendMessages();
	for each (Component* c in components)
		if (c->isEnable()) c->update();

	for each (Component* c in components)
		if (c->isEnable()) c->lateUpdate();
}

void Entity::create(std::vector<Component*>& newComponents)
{
	for (Component* newComponent : newComponents)
		addComponent(newComponent);
	init();
}

void Entity::addComponent(Component* newComponent)
{
	if (newComponent) {
		newComponent->setEntity(this);
		newComponent->awake();
		components.push_back(newComponent);
	}
}
void Entity::reciveMsg(Msg_Base* msg)
{
	messages.push(msg);
}

void Entity::sendMessages()
{
	while (!messages.empty()) {
		for each (Component* c in components)
		{
			c->listen(messages.front());
		}
		delete messages.front();
		messages.pop();
	}
}

Component* Entity::getComponent(ComponentType type) {
	for (Component* c : components)
	{
		if (c->getComponentType() == type)
			return c;
	}
	return nullptr;
}

void Entity::destroy()
{
	_alive = false;
}
