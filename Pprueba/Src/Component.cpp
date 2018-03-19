#include "Component.h"
#include "Entity.h"
#include "Scene.h"



Component::Component(Entity* myEntity_, const std::string componentName_) :
	_myEntity(myEntity_), _enable(true), componentName(componentName_)
{

}

Component::~Component()
{

}

void Component::sendMsg(Msg& newMsg)
{
	if (newMsg.reciver == newMsg.self)
		_myEntity->reciveMsg(newMsg);
	else
		_myEntity->getScene()->reciveMsg(newMsg);


}