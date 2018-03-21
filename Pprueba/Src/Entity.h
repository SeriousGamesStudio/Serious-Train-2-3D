#ifndef _H_M_ENTITY_H_
#define _H_M_ENTITY_H_

#include <vector>
#include <string>
#include <deque>
#include <typeinfo>

#include "Notifications.h"

class Component;
class Scene;
class Entity
{
public:
	Entity(Scene* scene_, EntityId id, std::string name);
	~Entity();

	void tick();

	void addComponent(Component* newComponent);
	void reciveMsg(Msg_Base* msg);

	inline	  EntityId const getId() { return _id; };
	inline std::string const getName() { return _name; }

	Component* getComponent(const std::string& componentName);

	inline Scene* getScene() { return _scene; };

private:
	Scene * _scene;
	EntityId _id;
	std::string _name;
	std::vector<Component*> components;
	std::deque<Msg_Base*> messages;
private:
	void sendMessages();
};

#endif //!_H_M_ENTITY_H_