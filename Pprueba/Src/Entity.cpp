#include "Entity.h"



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
void Entity::reciveMsg(Msg & msg)
{
	messages.push_back(msg);
}

