#include "Entity.h"



Entity::Entity(std::string id):id(id)
{
}


Entity::~Entity()
{
}

void Entity::update()
{
	for (auto component : components)
		component.update();
}
