#ifndef _H_COMPONENT_H_
#define _H_COMPONENT_H_

#include "Entity.h"

class Component
{
public:
	Component(Entity* myEntity_);
	virtual ~Component();

	virtual void awake() {};
	virtual void start() {};
	virtual void update() {};
	virtual void lateUpdate() {};

	virtual void listen(Msg& msg) {};

	//Getters and Setters
	inline bool isEnable() const { return _enable; }
	inline void enable() { _enable = true; }
	inline void disable() { _enable = false; }
	inline Entity* getEntity() const { return _myEntity; };
private:
	Entity * _myEntity;
	bool _enable;
private:
	void sendMsg(Msg& newMsg);
};
#endif //!_H_COMPONENT_H_

