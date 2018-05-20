#include "Entity.h"
#include "Scene.h"
#include "Component.h"


Entity::Entity(Scene* scene_, EntityId id_, std::string name_) :
	_scene(scene_), _id(id_), _name(name_)
{

}

Entity::~Entity()
{
	for (Component* c : components)
		delete c;
	while (!messages.empty()) {
		delete messages.front();
		messages.pop();
	}
}

void Entity::init()
{
	//get ride of extra space in component vector
	components.shrink_to_fit();
	//call the start method of every component
	for (Component* c : components)
		c->start();
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
	_scene->destroyEntity(this);
}
