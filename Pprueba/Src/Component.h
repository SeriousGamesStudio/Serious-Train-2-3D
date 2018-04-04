#ifndef _H_COMPONENT_H_
#define _H_COMPONENT_H_

#include <string>
#include "Notifications.h"
extern enum class ComponentType;
class Entity;
class Component
{
public:
	Component(Entity* my_Entity_, ComponentType type);
	virtual ~Component();

	virtual void awake() {};
	virtual void start() {};
	virtual void update() {};
	virtual void lateUpdate() {};

	virtual void listen(Msg_Base* msg) {};

	//Getters and Setters
	inline const ComponentType getComponentType() const { return type; };
	inline bool isEnable() const { return _enable; }
	inline void enable() { _enable = true; }
	inline void disable() { _enable = false; }
	inline Entity* getEntity() const { return _myEntity; };
private:
	bool _enable;
	ComponentType type;
protected:
	Entity * _myEntity;
	void sendMsg(Msg_Base* newMsg);
};
#endif //!_H_COMPONENT_H_

