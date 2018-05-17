#ifndef _H_M_ENTITY_H_
#define _H_M_ENTITY_H_

#include <vector>
#include <string>
#include <queue>
#include <typeinfo>

#include "Notifications.h"

enum ComponentType;
class Component;
class Scene;
class Entity
{
public:
	Entity(Scene* scene_, EntityId id, std::string name);
	~Entity();

	void init();
	void tick();

	void create(std::vector<Component*>& newComponents);

	void addComponent(Component* newComponent);
	void reciveMsg(Msg_Base* msg);

	inline	  EntityId const getId() { return _id; };
	inline std::string const getName() { return _name; }

	Component* getComponent(ComponentType type);

	inline Scene* getScene() { return _scene; };

private:
	Scene * _scene;
	EntityId _id;
	std::string _name;
	std::vector<Component*> components;
	std::queue<Msg_Base*> messages;
private:
	void sendMessages();
};

#endif //!_H_M_ENTITY_H_