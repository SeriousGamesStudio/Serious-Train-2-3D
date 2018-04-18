#ifndef _H_COMPONENT_H_
#define _H_COMPONENT_H_

#include <string>
#include "Notifications.h"
enum ComponentType;
class Entity;
class Component
{
public:
	/********************************************************
	* Component constructor.
	* params:
	* - type: enum that represents this kind of component
	* - typeString: string that repesents the kind of component
	*********************************************************/
	Component(ComponentType type, std::string typeString);
	virtual ~Component();
	/***********************************************************************************
	* Virtual method. It's called once when the entity owner of this component has value
	* usange: to init the attributes that need the entity
	************************************************************************************/
	virtual void awake() {};

	/*********************************************************************************************
	* Virtual method. It's called once when all components of the entity owner has been initialized
	* usange: to init the dependences with other components of the same entity through: _myEntity->getComponent()
	***********************************************************************************************/
	virtual void start() {};

	/********************************************************************************************
	* Virtual method. It's called once every frame
	* usange: to update this component state
	*********************************************************************************************/
	virtual void update() {};

	/********************************************************************************************
	* Virtual method. It's called once every frame but after every component has been updated
	* usange: to update this component state with a information that had to be calculated in a previous state
	*********************************************************************************************/
	virtual void lateUpdate() {};

	/*
	* Virtual method. Gets and parse messages from the entity owner and other entitys
	* params:
	* - msg: message to be parsed
	* usange: recive msgs that have to be parsed and then do whatever the component has to do with that information
	*/
	virtual void listen(Msg_Base* msg) {};

	//Getters and Setters
	//Gets componentType's enum
	inline const ComponentType getComponentType() const { return type; };

	//returns true if the component is enabled, false if not
	inline bool isEnable() const { return _enable; }

	//Enables component
	inline void enable() { _enable = true; }

	//Disbles component
	inline void disable() { _enable = false; }

	//Gets a pointer to the entity owner of the component
	inline Entity* getEntity() const { return _myEntity; };

private:
	bool _enable;
	ComponentType type;
	friend class Entity;
	inline void setEntity(Entity* entity) { _myEntity = entity; };
protected:
	Entity * _myEntity;

	//Sends a new message to the indicated entity
	void sendMsg(Msg_Base* newMsg);
};
#endif //!_H_COMPONENT_H_

