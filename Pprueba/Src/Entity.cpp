#include "Entity.h"
//#include "Scene.h"
#include "Component.h"


m_Entity::m_Entity(Scene* scene_, EntityId id_, std::string name_) :
	_scene(scene_), _id(id_), _name(name_)
{

}

m_Entity::~m_Entity()
{
	for (Component* c : components)
		delete c;
	components.clear();
	while (!messages.empty()) messages.pop_front();
}

void m_Entity::tick()
{
	for each (Component* c in components)
	{
		c->update();
		c->lateUpdate();
	}
}

void m_Entity::addComponent(Component* newComponent)
{
	if (newComponent)
		components.push_back(newComponent);
}
void m_Entity::reciveMsg(Msg & msg)
{
	messages.push_back(msg);
}

Component * m_Entity::getComponent(type_info componentType)
{
	for (Component* c : components)
		if (typeid(c) == componentType)
			return c;
	return nullptr;
}

