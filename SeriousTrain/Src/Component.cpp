#include "Component.h"
#include "Components.h"
#include "Entity.h"
#include "Scene.h"
#include "Notifications.h"

#include <string>

Component::Component(ComponentType type_) :
	_enable(true), type(type_)
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

Component * Component::getComponent(ComponentType type)
{
	return _myEntity->getComponent(type);
}

void Component::destroyMyEntity()
{
	_myEntity->destroy();
}
