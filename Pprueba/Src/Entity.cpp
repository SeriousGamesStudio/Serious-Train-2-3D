#include "Entity.h"
//#include "Scene.h"
#include "Component.h"


Entity::Entity(Scene* scene_, EntityId id_, std::string name_) :
	_scene(scene_), _id(id_), _name(name_)
{

}

Entity::~Entity()
{
	for (Component* c : components)
		delete c;
	components.clear();
	while (!messages.empty()) messages.pop_front();
}

void Entity::tick()
{
	sendMessages();
	for each (Component* c in components)
	{
		c->update();
		c->lateUpdate();
	}
}

void Entity::addComponent(Component* newComponent)
{
	if (newComponent)
		components.push_back(newComponent);
}
void Entity::reciveMsg(Msg_Base* msg)
{
	messages.push_back(msg);
}

void Entity::sendMessages()
{
	while (!messages.empty()) {
		for each (Component* c in components)
		{
			c->listen(messages.front());
		}
		delete messages.front();
		messages.pop_front();
	}
}

Component* Entity::getComponent(const std::string& componentName) {
	for (Component* c : components)
	{
		if (c->getComponentNamme() == componentName)
			return c;
	}
	return nullptr;
}
