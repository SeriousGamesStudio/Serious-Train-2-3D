#include "Component.h"
#include "Entity.h"
#include "Notifications.h"



Component::Component(Entity* myEntity_) :
	_myEntity(myEntity_), _enable(true)
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
