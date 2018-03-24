#include "Component.h"
#include "Entity.h"
#include "Scene.h"
#include "Notifications.h"



Component::Component(Entity* myEntity_, ComponentType type_) :
	_myEntity(myEntity_), _enable(true), type(type_)
{

}

Component::~Component()
{

}

void Component::sendMsg(Msg_Base* newMsg)
{
	if (newMsg->reciver == Msg_Base::self)
		_myEntity->reciveMsg(newMsg);
	else
		_myEntity->getScene()->reciveMsg(newMsg);


}