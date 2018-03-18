#ifndef _H_COMPONENT_H_
#define _H_COMPONENT_H_

#include "Notifications.h"
class m_Entity;

class Component
{
public:
	Component(m_Entity* my_Entity_);
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
	inline m_Entity* getEntity() const { return _myEntity; };
private:
	m_Entity * _myEntity;
	bool _enable;
protected:
	void sendMsg(Msg& newMsg);
};
#endif //!_H_COMPONENT_H_

