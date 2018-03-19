#ifndef _H_COMPONENT_H_
#define _H_COMPONENT_H_

#include <string>
#include "Notifications.h"
class Entity;

class Component
{
public:
	Component(Entity* my_Entity_, const std::string componentName);
	virtual ~Component();

	virtual void awake() {};
	virtual void start() {};
	virtual void update() {};
	virtual void lateUpdate() {};

	virtual void listen(Msg& msg) {};

	//Getters and Setters
	inline const std::string&  getComponentNamme() const { return componentName; };
	inline bool isEnable() const { return _enable; }
	inline void enable() { _enable = true; }
	inline void disable() { _enable = false; }
	inline Entity* getEntity() const { return _myEntity; };
private:
	bool _enable;
	std::string componentName;
protected:
	Entity * _myEntity;
	void sendMsg(Msg& newMsg);
};
#endif //!_H_COMPONENT_H_

