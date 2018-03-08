#ifndef _H_ENTITY_H_
#define _H_ENTITY_H_

#include <vector>
#include <string>
#include <deque>

#include "Notifications.h"

using EntityId = unsigned int;
class Component;
class Scene;
class Entity
{
public:
	Entity(Scene* scene_, EntityId id, std::string name);
	~Entity();

	void tick();

	void addComponent(Component* newComponent);
	void reciveMsg(Msg& msg);

	inline	  EntityId const getId() { return _id; };
	inline std::string const getName() { return _name; }

	/* preguntar se se puede hacer esto, en caso contrario preguntar cómo se podría hacer
	template<class T>
	inline Component* Entity::getComponent<T>()
	{
		for (Component* c : component)
			if (dynamic_cast<T>(c))
				return c;

		return nullptr;
	};*/

	inline Scene* getScene() { return _scene; };

private:
	Scene * _scene;
	EntityId _id;
	std::string _name;
	std::vector<Component*> components;
	std::deque<Msg> messages;
};

#endif //!_H_ENTITY_H_